#include <OpenGP/GL/Application.h>
#include "OpenGP/GL/Eigen.h"

#include "noise.h"

using namespace OpenGP;
const int width=800, height=600;

const char* vshader =
#include "vshader.glsl"
;
const char* fshader =
#include "fshader.glsl"
;

void init();

std::unique_ptr<Shader> shader;
std::unique_ptr<GPUMesh> quad;
std::unique_ptr<R32FTexture> heightTexture;

int main(int, char**){

    Application app;

    init();\

    ///--- Get height texture
    heightTexture = std::unique_ptr<R32FTexture>(perlin2DTexture());

    // Display callback
    Window& window = app.create_window([&](Window&){
        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();

        glActiveTexture(GL_TEXTURE0);
        heightTexture->bind();
        shader->set_uniform("noiseTex", 0);

        quad->set_mode(GL_TRIANGLE_STRIP);
        quad->set_attributes(*shader);
        quad->draw();

        heightTexture->unbind();
        shader->unbind();

    });
    window.set_title("Noise and Normals");
    window.set_size(width, height);


    return app.run();
}

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );

    ///--- Compile shaders
    shader = std::unique_ptr<Shader>(new Shader());
    shader->verbose = true;
    shader->add_vshader_from_source(vshader);
    shader->add_fshader_from_source(fshader);
    shader->link();

    ///--- Quad mesh
    quad = std::unique_ptr<GPUMesh>(new GPUMesh());
    std::vector<Vec2> points;
    points.push_back(Vec2( 1,-1));
    points.push_back(Vec2( 1, 1));
    points.push_back(Vec2(-1,-1));
    points.push_back(Vec2(-1, 1));
    std::vector<Vec2> texCoords;
    texCoords.push_back(Vec2(1,0));
    texCoords.push_back(Vec2(1,1));
    texCoords.push_back(Vec2(0,0));
    texCoords.push_back(Vec2(0,1));
    std::vector<unsigned int> indices = { 0,1,2,3 }; // Using GL_TRIANGLE_STRIP
    quad->set_vbo<Vec2>("vposition", points);
    quad->set_vtexcoord(texCoords);
    quad->set_triangles(indices);
}
