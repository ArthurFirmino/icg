#include <OpenGP/GL/Application.h>

const char *vshader =
#include "vshader.glsl"
;
const char *fshader =
#include "fshader.glsl"
;

using namespace OpenGP;

constexpr int width = 600;
constexpr int height = 600;

std::unique_ptr<Shader> shader;
std::unique_ptr<GPUMesh> fsquad;

void init();

int main() {

    Application app;
    init();

    Window& window = app.create_window([](Window&){
        glViewport(0, 0, width, height);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
        shader->set_uniform("resolution_w", float(width));
        shader->set_uniform("resolution_h", float(height));
        fsquad->set_attributes(*shader);
        fsquad->draw();
        shader->unbind();
    });
    window.set_title("Super Sampling AA Example");
    window.set_size(width, height);

    return app.run();
}

void init() {

    shader = std::unique_ptr<Shader>(new Shader());
    shader->add_vshader_from_source(vshader);
    shader->add_fshader_from_source(fshader);
    shader->link();

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
}
