#include <OpenGP/GL/Application.h>
#include "OpenGP/GL/glfw_helpers.h"
using namespace OpenGP;

constexpr int width = 600;
constexpr int height = 600;

std::unique_ptr<Shader> SSAAshader;
std::unique_ptr<Shader> MSAAshader;

const char *ssaa_vshader =
#include "ssaa_vshader.glsl"
;
const char *ssaa_fshader =
#include "ssaa_fshader.glsl"
;
const char *msaa_vshader =
#include "msaa_vshader.glsl"
;
const char *msaa_fshader =
#include "msaa_fshader.glsl"
;

std::unique_ptr<GPUMesh> fsquad;
std::unique_ptr<GPUMesh> triangle;

void init();

int main() {
    Application app;
    init();

    glfwWindowHint(GLFW_SAMPLES, 0);
    Window& windowSSAA = app.create_window([](Window&){
        glViewport(0, 0, width, height);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SSAAshader->bind();
        fsquad->set_attributes(*SSAAshader);
        fsquad->draw();
        SSAAshader->unbind();
    });
    windowSSAA.set_title("Super Sampling AA Example");
    windowSSAA.set_size(width, height);

    //glfwWindowHint(GLFW_SAMPLES, 16);
    Window& windowMSAA = app.create_window([](Window&){
        //glEnable(GL_MULTISAMPLE);
        glViewport(0, 0, width, height);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        MSAAshader->bind();
        triangle->set_attributes(*MSAAshader);
        triangle->draw();
        MSAAshader->unbind();
        glDisable(GL_MULTISAMPLE);
    });
    windowMSAA.set_title("Multi Sampling AA Example");
    windowMSAA.set_size(width, height);

    //std::cout << "\nRunning Application" << std::endl;
    return app.run();
}

void init() {

    SSAAshader = std::unique_ptr<Shader>(new Shader());
    //SSAAshader->verbose = true;
    SSAAshader->add_vshader_from_source(ssaa_vshader);
    SSAAshader->add_fshader_from_source(ssaa_fshader);
    SSAAshader->link();

    MSAAshader = std::unique_ptr<Shader>(new Shader());
    //MSAAshader->verbose = true;
    MSAAshader->add_vshader_from_source(msaa_vshader);
    MSAAshader->add_fshader_from_source(msaa_fshader);
    MSAAshader->link();

    fsquad = std::unique_ptr<GPUMesh>(new GPUMesh());
    std::vector<Vec3> fsquad_vposition = {
        Vec3(-1, -1, 0),
        Vec3(-1,  1, 0),
        Vec3( 1, -1, 0),
        Vec3( 1,  1, 0)
    };
    fsquad->set_vbo<Vec3>("vposition", fsquad_vposition);
    std::vector<unsigned int> fsquad_triangle_indices = {
        0, 2, 1, 1, 2, 3
    };
    fsquad->set_triangles(fsquad_triangle_indices);
    std::vector<Vec2> fsquad_vtexcoord = {
        Vec2(-1, -1),
        Vec2(-1,  1),
        Vec2( 1, -1),
        Vec2( 1,  1)
    };
    fsquad->set_vtexcoord(fsquad_vtexcoord);

    triangle = std::unique_ptr<GPUMesh>(new GPUMesh());
    std::vector<Vec3> triangle_vposition = {
        Vec3(-0.5f,     0, 0),
        Vec3( 0.3f, -0.3f, 0),
        Vec3( 0.5f,  0.3f, 0)
    };
    triangle->set_vbo<Vec3>("vposition", triangle_vposition);
    std::vector<unsigned int> triangle_indices = {
        0, 1, 2
    };
    triangle->set_triangles(triangle_indices);
}
