/*
 * CSC 305 201801 UVIC
 * The purpose of this source file is to demonstrate the Mesh class which you may use in assignment 2
 * Its only functionality is to render vertices/normals/textures and load textures from png files
 * A demonstration of an ImGui menu window is also included in this file
*/
#include "Mesh/Mesh.h"
#include "OpenGP/GL/glfw_helpers.h"

#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/Application.h>
#include <OpenGP/GL/ImguiRenderer.h>

using namespace OpenGP;

int main() {

    Application app;
    ImguiRenderer imrenderer;
    Mesh renderMesh;

    /// Example rendering a mesh
    /// Call to compile shaders
    renderMesh.init();

    /// Load Vertices and Indices (minimum required for Mesh::draw to work)
    std::vector<Vec3> vertList;
    vertList.push_back(Vec3(0,0,0));
    vertList.push_back(Vec3(1,0,0));
    vertList.push_back(Vec3(1,1,0));
    vertList.push_back(Vec3(0,1,0));
    std::vector<unsigned int> indexList;
    indexList.push_back(0); // Face 1
    indexList.push_back(1);
    indexList.push_back(2);
    indexList.push_back(2); // Face 2
    indexList.push_back(3);
    indexList.push_back(0);
    renderMesh.loadVertices(vertList, indexList);

    /// Load normals
    std::vector<Vec3> normList;
    normList.push_back(Vec3(0,0,1));
    normList.push_back(Vec3(0,0,1));
    normList.push_back(Vec3(0,0,1));
    normList.push_back(Vec3(0,0,1));
    renderMesh.loadNormals(normList);

    /// Load textures (assumes texcoords)
    renderMesh.loadTextures("earth.png");

    /// Load texture coordinates (assumes textures)
    std::vector<Vec2> tCoordList;
    tCoordList.push_back(Vec2(0,0));
    tCoordList.push_back(Vec2(1,0));
    tCoordList.push_back(Vec2(1,1));
    tCoordList.push_back(Vec2(0,1));
    renderMesh.loadTexCoords(tCoordList);

    /// Create main window, set callback function
    auto &window1 = app.create_window([&](Window &window){
        int width, height;
        std::tie(width, height) = window.get_size();

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// Wireframe rendering, might be helpful when debugging your mesh generation
        // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        float ratio = width / (float) height;
        Mat4x4 modelTransform = Mat4x4::Identity();
        Mat4x4 model = modelTransform.matrix();
        Mat4x4 projection = OpenGP::perspective(70.0f, ratio, 0.1f, 10.0f);

        //camera movement
        float time = .5f * (float)glfwGetTime();
        Vec3 cam_pos(2*cos(time), 2.0, 2*sin(time));
        Vec3 cam_look(0.0f, 0.0f, 0.0f);
        Vec3 cam_up(0.0f, 1.0f, 0.0f);
        Mat4x4 view = OpenGP::lookAt(cam_pos, cam_look, cam_up);

        renderMesh.draw(model, view, projection);
    });
    window1.set_title("Assignment 2");

    /// Create window for IMGUI, set callback function
    auto &window2 = app.create_window([&](Window &window){
        int width, height;
        std::tie(width, height) = window.get_size();

        imrenderer.begin_frame(width, height);

        ImGui::BeginMainMenuBar();
        ImGui::MenuItem("File");
        ImGui::MenuItem("Edit");
        ImGui::MenuItem("View");
        ImGui::MenuItem("Help");
        ImGui::EndMainMenuBar();

        ImGui::Begin("Test Window 1");
        ImGui::Text("This is a test imgui window");
        ImGui::End();

        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        imrenderer.end_frame();
    });
    window2.set_title("imgui Test");

    return app.run();
}
