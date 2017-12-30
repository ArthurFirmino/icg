#pragma once

#include <memory>

#include "icg_common.h"

typedef Eigen::Matrix<uint8_t, 3, 1> Colour;


class Image {
private:

    GLuint gpuTex;
    GLuint program;

    const char *fshader_source =
            "#version 330 core\n"
            "out vec3 color;"
            "in vec2 uv;"
            "uniform sampler2D tex;"
            "void main() {"
            "    color = texture(tex,uv).rgb;\n"
            "}";

    const char *vshader_source =
            "#version 330 core\n"
            "in vec3 vpoint;"
            "in vec2 vtexcoord;"
            "out vec2 uv;"
            "void main() {"
            "    gl_Position = vec4(vpoint, 1.0);"
            "    uv = vtexcoord;"
            "}";

public:

    int rows, cols;

    std::unique_ptr<uint8_t[]> data;
    std::unique_ptr<float[]> depth;

    Image(int cols, int rows) {

        this->rows = rows;
        this->cols = cols;

        data = std::unique_ptr<uint8_t[]>(new uint8_t[3 * cols * rows]);
        depth = std::unique_ptr<float[]>(new float[cols * rows]);

    }

    Colour get(int row, int col) {

        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);

        auto b = data[3 * (row * cols + col)];
        auto g = data[3 * (row * cols + col) + 1];
        auto r = data[3 * (row * cols + col) + 2];

        return Colour(r, g, b);

    }

    float getDepth(int row, int col) {

        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);

        return depth[row * cols + col];

    }

    void set(int row, int col, Colour colour) {

        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);

        data[3 * (row * cols + col)]     = colour(2);
        data[3 * (row * cols + col) + 1] = colour(1);
        data[3 * (row * cols + col) + 2] = colour(0);

    }

    void setDepth(int row, int col, float val) {

        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);

        depth[row * cols + col] = val;

    }

    void save(std::string name) {

        /// http://stackoverflow.com/a/2654860

        FILE *f;
        unsigned char *img = data.get();

        int w = cols;
        int h = rows;

        int filesize = 54 + 3*w*h;

        unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
        unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
        unsigned char bmppad[3] = {0,0,0};

        bmpfileheader[ 2] = (unsigned char)(filesize    );
        bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
        bmpfileheader[ 4] = (unsigned char)(filesize>>16);
        bmpfileheader[ 5] = (unsigned char)(filesize>>24);

        bmpinfoheader[ 4] = (unsigned char)(       w    );
        bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
        bmpinfoheader[ 6] = (unsigned char)(       w>>16);
        bmpinfoheader[ 7] = (unsigned char)(       w>>24);
        bmpinfoheader[ 8] = (unsigned char)(       h    );
        bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
        bmpinfoheader[10] = (unsigned char)(       h>>16);
        bmpinfoheader[11] = (unsigned char)(       h>>24);

        f = fopen(name.c_str(),"wb");
        fwrite(bmpfileheader,1,14,f);
        fwrite(bmpinfoheader,1,40,f);
        for(int i=0; i<h; i++)
        {
            fwrite(img+(w*i*3),3,w,f);
            fwrite(bmppad,1,(4-(w*3)%4)%4,f);
        }
        fclose(f);

    }

    void show() {

        OpenGP::glfwInitWindowSize(cols,rows);
        OpenGP::glfwMakeWindow("Output Image");

        program = OpenGP::compile_shaders(vshader_source, fshader_source);

        glUseProgram(program);

        /// Texture
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &gpuTex);
        glBindTexture(GL_TEXTURE_2D, gpuTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_BGR, GL_UNSIGNED_BYTE, data.get());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLint tex_id = glGetUniformLocation(program, "tex");
        glUniform1i(tex_id, 0);

        /// Vertex Data
        GLuint _vao, _vbo_vpoint, _vbo_vtexcoord;

        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        ///--- Vertex coordinates
        {
            const GLfloat vpoint[] = { -1.0f, -1.0f, 0.0f,
                                       +1.0f, -1.0f, 0.0f,
                                       -1.0f, +1.0f, 0.0f,
                                       +1.0f, +1.0f, 0.0f };
            ///--- Buffer
            glGenBuffers(1, &_vbo_vpoint);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(program, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- Texture coordinates
        {
            const GLfloat vtexcoord[] = { 0.0f, 0.0f,
                                          1.0f, 0.0f,
                                          0.0f, 1.0f,
                                          1.0f, 1.0f};

            ///--- Buffer
            glGenBuffers(1, &_vbo_vtexcoord);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_vtexcoord);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);

            ///--- Attribute
            GLuint vtexcoord_id = glGetAttribLocation(program, "vtexcoord");
            glEnableVertexAttribArray(vtexcoord_id);
            glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        OpenGP::glfwDisplayFunc(&display);

        glBindVertexArray(_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gpuTex);
        glUseProgram(program);

        OpenGP::glfwMainLoop();

    }

    private: static void display() {

        glClearColor(0.0,0.0,0.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ///--- Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    }

};
