#include <OpenGP/GL/Application.h>
#include "OpenGP/GL/Eigen.h"

using namespace OpenGP;
const int width=1280, height=720;

const char* terrain_vshader =
#include "terrain_vshader.glsl"
;
const char* terrain_fshader =
#include "terrain_fshader.glsl"
;

void init();
void genTerrainMesh();
void drawTerrain();

std::unique_ptr<Shader> terrainShader;
std::unique_ptr<GPUMesh> terrainMesh;

int main(int, char**){

    Application app;

    init();
    genTerrainMesh();

    // Display callback
    Window& window = app.create_window([&](Window&){
        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawTerrain();
    });
    window.set_title("vertex_displacement");
    window.set_size(width, height);

    return app.run();
}

void init(){
    glClearColor(0.1,0.1,0.1, /*solid*/1.0 );

    ///--- Compile shaders
    terrainShader = std::unique_ptr<Shader>(new Shader());
    terrainShader->verbose = true;
    terrainShader->add_vshader_from_source(terrain_vshader);
    terrainShader->add_fshader_from_source(terrain_fshader);
    terrainShader->link();
}

void genTerrainMesh() {
    /// NOTE: For assignment 4 you cannot use this method to generate the mesh, you'll have to use GL_TRIANGLE_STRIPS
    /// Create a flat (z=0) mesh for the terrain with given dimensions
    terrainMesh = std::unique_ptr<GPUMesh>(new GPUMesh());
    int n_width = 16; // Grid resolution
    int n_height = 16;

    std::vector<Vec3> points;
    std::vector<unsigned int> indices;
    std::vector<Vec2> texCoords;

    ///--- Vertex positions
    for(int j=0; j<n_height; ++j) {
        for(int i=0; i<n_width; ++i) {
            points.push_back(Vec3(i/(float)n_width, j/(float)n_height, 0.0f));
            texCoords.push_back(Vec2( i/(float)(n_width-1), j/(float)(n_height-1) ));
        }
    }

    ///--- Element indices using GL_TRIANGLE
    for(int j=0; j<n_height-1; ++j) {
        for(int i=0; i<n_width-1; ++i) {
            /// Triangle 1
            indices.push_back(i*n_height + j);
            indices.push_back((i+1)*n_height + j);
            indices.push_back(i*n_height + (j+1));
            /// Triangle 2
            indices.push_back(i*n_height + (j+1));
            indices.push_back((i+1)*n_height + j);
            indices.push_back((i+1)*n_height + (j+1));
        }
    }

    terrainMesh->set_vbo<Vec3>("vposition", points);
    terrainMesh->set_vbo<Vec2>("vtexcoord", texCoords);
    terrainMesh->set_triangles(indices);
}

void drawTerrain() {
    terrainShader->bind();

    /// Create transformation matrices, use lookAt and perspective
    Mat4x4 M = Mat4x4::Identity(); // Identity should be fine
    terrainShader->set_uniform("M", M);

    Mat4x4 V = lookAt(Vec3(0.5,0,0.5), Vec3(0.5,0.5,0), Vec3(0,0,1));
    terrainShader->set_uniform("V", V);

    Mat4x4 P = perspective(70.0f, width / (float)height, 0.01f, 10.0f);
    terrainShader->set_uniform("P", P);

    // Draw terrain
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    terrainMesh->set_attributes(*terrainShader);
    terrainMesh->draw();

    terrainShader->unbind();
}
