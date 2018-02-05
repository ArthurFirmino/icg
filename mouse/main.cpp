#include <OpenGP/GL/Eigen.h>

#include "_quad/Quad.h"
#include "_point/point.h"
#include "_multiline/multiline.h"

using namespace OpenGP;

int window_width = 1024;
int window_height = 768;

Mat4x4 projection;
Mat4x4 view;
Mat4x4 model;

GLuint _pid_multiline;
GLuint _pid_point;
GLuint _pid_point_selection;

Quad quad;

MultiLine cam_pos_curve;

std::vector<ControlPoint> cam_pos_points;
int selected_point;
double x_last, y_last;
void init(){
    /// Compile the shaders here to avoid the duplication
    _pid_multiline = OpenGP::load_shaders("_multiline/line_vshader.glsl", "_multiline/line_fshader.glsl");
    if(!_pid_multiline) exit(EXIT_FAILURE);

    _pid_point = OpenGP::load_shaders("_point/point_vshader.glsl", "_point/point_fshader.glsl");
    if(!_pid_point) exit(EXIT_FAILURE);

    _pid_point_selection = OpenGP::load_shaders("_point/point_selection_vshader.glsl", "_point/point_selection_fshader.glsl");
    if(!_pid_point_selection) exit(EXIT_FAILURE);

    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    quad.init();

    ///--- init cam_pos_curve
    cam_pos_curve.init(_pid_multiline);
    cam_pos_points.push_back(ControlPoint(-0.79, 0.09, 0.2, 0));
    cam_pos_points.push_back(ControlPoint(-0.88, -0.71, 0.2, 1));
    cam_pos_points.push_back(ControlPoint(1.3, -0.8, 0.2, 2));
    cam_pos_points.push_back(ControlPoint(0.71, 0.76, 0.2, 3));
    for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].id() = i;
        cam_pos_points[i].init(_pid_point, _pid_point_selection);
    }

    cam_pos_curve.set_points(cam_pos_points[0].position(), cam_pos_points[1].position(), cam_pos_points[2].position(), cam_pos_points[3].position());

    ///--- Setup view-projection matrix
    GLfloat top = 1.0f;
    GLfloat right = (GLfloat)window_width / window_height * top;

    projection = OpenGP::ortho(-right, right, -top, top, -10.0f, 10.0f);

    Vec3 cam_pos(0.0f, 0.0f, 2.0f);
    Vec3 cam_look(0.0f, 0.0f, 0.0f);
    Vec3 cam_up(0.0f, 1.0f, 0.0f);

    view = OpenGP::lookAt(cam_pos, cam_look, cam_up);
    model = Mat4x4::Identity();
    selected_point = -1;
    x_last = 0;
    y_last = 0;
}

bool unproject (int mouse_x, int mouse_y, Vec3 &p) {
    ///---This only works with the specific projection, model and view matrices defined in init().
    p[0] =  ((float)mouse_x - (float) window_width / 2.0f) / (float) window_height / 0.5;
    p[1] =  ((float) window_height/2 - (float) mouse_y) / (float) window_height / 0.5;
    return true;
}

void display(){
    glViewport(0,0,window_width,window_height);
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //quad.draw(Mat4x4::Identity(), Mat4x4::Identity(), Mat4x4::Identity());
    for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].draw(model, view, projection);
    }
    cam_pos_curve.draw(model, view, projection);
}

void render_selection() {
    glViewport(0,0,window_width,window_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].draw_selection(model, view, projection);
    }
}

void selection_button(GLFWwindow *window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x = 0, y = 0;
        glfwGetCursorPos(OpenGP::window, &x, &y);
        x_last = x; y_last = y;
        //render the image with color code
        render_selection();

        glFlush();
        glFinish();
        //picking pixel
        unsigned char res[4];
        glReadPixels(x, window_height - y, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
        selected_point = res[0];

        if (selected_point >= 0 && selected_point < cam_pos_points.size())
            cam_pos_points[selected_point].selected() = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        if (selected_point >= 0 && selected_point < cam_pos_points.size()) {
            cam_pos_points[selected_point].selected() = false;

        double x = 0, y = 0;
        glfwGetCursorPos(OpenGP::window, &x, &y);
        //if the mouse is not moved at all, don't bother redraw
        if (x != x_last || y != y_last) {
        //caculate the world coordiante from the pixel coordinate on screen
        unproject(x, y, cam_pos_points[selected_point].position());
        cam_pos_curve.set_points(cam_pos_points[0].position(),
                cam_pos_points[1].position(),
                cam_pos_points[2].position(),
                cam_pos_points[3].position());
            }
        }
        selected_point = -1;
    }
}

void mousemove(GLFWwindow *window, double x, double y) {

    //TODO: make the curve change while holding down a mouse button and
    //move the mouse!

}

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(action != GLFW_RELEASE) return; ///< only act on key release
    switch(key){
        default:
            break;
    }
}

void cleanup(){
    glDeleteProgram(_pid_multiline);
    glDeleteProgram(_pid_point);
    glDeleteProgram(_pid_point_selection);
}

int main(int, char**){
    OpenGP::glfwInitWindowSize(window_width, window_height);
    OpenGP::glfwMakeWindow("Mouse");
    OpenGP::glfwDisplayFunc(display);
    glfwSetKeyCallback(OpenGP::window, keyboard);
    glfwSetMouseButtonCallback(OpenGP::window, selection_button);
    glfwSetCursorPosCallback(OpenGP::window, mousemove);
    init();
    OpenGP::glfwMainLoop();
    cleanup();
    return EXIT_SUCCESS;
}
