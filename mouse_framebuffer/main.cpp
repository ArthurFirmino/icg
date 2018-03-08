#include <OpenGP/GL/Application.h>

using namespace OpenGP;

const int width=720, height=720;
#define POINTSIZE 10.0f

const char* line_vshader =
#include "line_vshader.glsl"
;
const char* line_fshader =
#include "line_fshader.glsl"
;
const char* selection_vshader =
#include "selection_vshader.glsl"
;
const char* selection_fshader =
#include "selection_fshader.glsl"
;

void init();
std::unique_ptr<Shader> lineShader;
std::unique_ptr<GPUMesh> line;
std::vector<Vec2> controlPoints;

/// Selection with framebuffer pointers
std::unique_ptr<Shader> selectionShader;
std::unique_ptr<Framebuffer> selectionFB;
std::unique_ptr<RGBA8Texture> selectionColor;
std::unique_ptr<D16Texture> selectionDepth;

int main(int, char**){

    Application app;
    init();

    /// Selection shader
    selectionShader = std::unique_ptr<Shader>(new Shader());
    selectionShader->verbose = true;
    selectionShader->add_vshader_from_source(selection_vshader);
    selectionShader->add_fshader_from_source(selection_fshader);
    selectionShader->link();
    /// Framebuffer for selection shader
    selectionFB = std::unique_ptr<Framebuffer>(new Framebuffer());
    selectionColor = std::unique_ptr<RGBA8Texture>(new RGBA8Texture());
    selectionColor->allocate(width,height);
    selectionDepth = std::unique_ptr<D16Texture>(new D16Texture());
    selectionDepth->allocate(width,height);
    selectionFB->attach_color_texture(*selectionColor);
    selectionFB->attach_depth_texture(*selectionDepth);

    // Mouse position and selected point
    Vec2 pixelPosition = Vec2(0,0);
    Vec2 position = Vec2(0,0);
    Vec2 *selection = nullptr;
    int offsetID = 0;

    // Display callback
    Window& window = app.create_window([&](Window&){
        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(POINTSIZE);

        lineShader->bind();

        // Draw line red
        lineShader->set_uniform("selection", -1);
        line->set_attributes(*lineShader);
        line->set_mode(GL_LINE_STRIP);
        line->draw();

        // Draw points red and selected point blue
        if(selection!=nullptr) lineShader->set_uniform("selection", int(selection-&controlPoints[0]));
        line->set_mode(GL_POINTS);
        line->draw();

        lineShader->unbind();
    });
    window.set_title("Mouse");
    window.set_size(width, height);

    // Mouse movement callback
    window.add_listener<MouseMoveEvent>([&](const MouseMoveEvent &m){
        // Mouse position in clip coordinates
        pixelPosition = m.position;
        Vec2 p = 2.0f*(Vec2(m.position.x()/width,-m.position.y()/height) - Vec2(0.5f,-0.5f));
        if( selection && (p-position).norm() > 0.0f) {
            selection->x() = position.x();
            selection->y() = position.y();
            line->set_vbo<Vec2>("vposition", controlPoints);
        }
        position = p;
    });

    // Mouse click callback
    window.add_listener<MouseButtonEvent>([&](const MouseButtonEvent &e){
        // Mouse selection case
        if( e.button == GLFW_MOUSE_BUTTON_LEFT && !e.released) {

            /// Draw element id's to framebuffer
            selectionFB->bind();
            glViewport(0,0,width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color must be 1,1,1,1
            glPointSize(POINTSIZE);
            selectionShader->bind();
            selectionShader->set_uniform("offsetID", offsetID);
            line->set_attributes(*selectionShader);
            line->set_mode(GL_POINTS);
            line->draw();
            selectionShader->unbind();
            glFlush();
            glFinish();

            selection = nullptr;
            unsigned char a[4];
            glReadPixels(int(pixelPosition[0]), height - int(pixelPosition[1]), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &a);
            selection = &controlPoints[0] + (int)a[0];
            selectionFB->unbind();
        }
        // Mouse release case
        if( e.button == GLFW_MOUSE_BUTTON_LEFT && e.released) {
            if(selection) {
                selection->x() = position.x();
                selection->y() = position.y();
                selection = nullptr;
                line->set_vbo<Vec2>("vposition", controlPoints);
            }
        }
    });

    return app.run();
}

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );

    lineShader = std::unique_ptr<Shader>(new Shader());
    lineShader->verbose = true;
    lineShader->add_vshader_from_source(line_vshader);
    lineShader->add_fshader_from_source(line_fshader);
    lineShader->link();

    controlPoints = std::vector<Vec2>();
    controlPoints.push_back(Vec2(-0.7f,-0.2f));
    controlPoints.push_back(Vec2(-0.3f, 0.2f));
    controlPoints.push_back(Vec2( 0.3f, 0.5f));
    controlPoints.push_back(Vec2( 0.7f, 0.0f));

    line = std::unique_ptr<GPUMesh>(new GPUMesh());
    line->set_vbo<Vec2>("vposition", controlPoints);
    std::vector<unsigned int> indices = {0,1,2,3};
    line->set_triangles(indices);
}
