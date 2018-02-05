#include "OpenGP/GL/Eigen.h"
#include "OpenGP/GL/glfw_helpers.h"
#include "Mesh/Mesh.h"

using namespace OpenGP;

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

const int SUN_ROT_PERIOD = 30;        
const int EARTH_ROT_PERIOD = 4;       
const int MOON_ROT_PERIOD = 8;       
const int EARTH_ORBITAL_PERIOD = 10; 
const int MOON_ORBITAL_PERIOD = 5;   
const int SPEED_FACTOR = 1;
    
Mesh sun;
Mesh earth;
Mesh moon;

void init();

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    float time_s = glfwGetTime();

    //TODO: Set up the transform hierarchies for the three objects!
    float freq = M_PI*time_s*SPEED_FACTOR;

    // **** Sun transform
    Transform sun_M = Transform::Identity();

    // **** Earth transform
    Transform earth_M = Transform::Identity();

    // **** Moon transform
    Transform moon_M = Transform::Identity();

    // draw the sun, the earth and the moon
    sun.draw(sun_M.matrix());
    earth.draw(earth_M.matrix());
    moon.draw(moon_M.matrix());
}

int main(int, char**){
    glfwInitWindowSize(512, 512);
    glfwMakeWindow("Planets");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}

void init(){
    glClearColor(1.0f,1.0f,1.0f, 1.0 );

    // Enable alpha blending so texture backgroudns remain transparent
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sun.init();
    earth.init();
    moon.init();

    std::vector<OpenGP::Vec3> quadVert;
    quadVert.push_back(OpenGP::Vec3(-1.0f, -1.0f, 0.0f));
    quadVert.push_back(OpenGP::Vec3(1.0f, -1.0f, 0.0f));
    quadVert.push_back(OpenGP::Vec3(1.0f, 1.0f, 0.0f));
    quadVert.push_back(OpenGP::Vec3(-1.0f, 1.0f, 0.0f));
    std::vector<unsigned> quadInd;
    quadInd.push_back(0);
    quadInd.push_back(1);
    quadInd.push_back(2);
    quadInd.push_back(0);
    quadInd.push_back(2);
    quadInd.push_back(3);
    sun.loadVertices(quadVert, quadInd);
    earth.loadVertices(quadVert, quadInd);
    moon.loadVertices(quadVert, quadInd);

    std::vector<OpenGP::Vec2> quadTCoord;
    quadTCoord.push_back(OpenGP::Vec2(0.0f, 0.0f));
    quadTCoord.push_back(OpenGP::Vec2(1.0f, 0.0f));
    quadTCoord.push_back(OpenGP::Vec2(1.0f, 1.0f));
    quadTCoord.push_back(OpenGP::Vec2(0.0f, 1.0f));
    sun.loadTexCoords(quadTCoord);
    earth.loadTexCoords(quadTCoord);
    moon.loadTexCoords(quadTCoord);

    sun.loadTextures("sun.png");
    moon.loadTextures("moon.png");
    earth.loadTextures("earth.png");
}
