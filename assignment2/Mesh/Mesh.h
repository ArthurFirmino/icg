#pragma once
#include <cassert>

#include "OpenGP/Image/Image.h"
#include "OpenGP/GL/Eigen.h"
#include "OpenGP/GL/check_error_gl.h"
#include "OpenGP/GL/shader_helpers.h"
#include "OpenGP/SurfaceMesh/SurfaceMesh.h"
#include "OpenGP/external/LodePNG/lodepng.cpp"

class Mesh{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID
    GLuint _texture;    ///< Texture buffer
    GLuint _vpoint;    ///< vertices buffer
    GLuint _vnormal;   ///< normals buffer
    GLuint _tcoord;    ///< texture coordinates buffer

    unsigned int numVertices;

    bool hasNormals;
    bool hasTextures;
    bool hasTexCoords;

public:

    GLuint getProgramID(){ return _pid; }

    void init() {
        ///--- Compile the shaders
        _pid = OpenGP::load_shaders("Mesh_vshader.glsl", "Mesh_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);
        check_error_gl();

        numVertices = 0;

        hasNormals = false;
        hasTextures = false;
        hasTexCoords = false;
    }

    void loadVertices(const std::vector<OpenGP::Vec3> &vertexArray, const std::vector<unsigned int> &indexArray) {
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Buffer
        glGenBuffers(1, &_vpoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(OpenGP::Vec3), &vertexArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3 /*vec3*/, GL_FLOAT, GL_FALSE /*DONT_NORMALIZE*/, 3*sizeof(float) /*STRIDE*/, (void*)0 /*ZERO_BUFFER_OFFSET*/);
        check_error_gl();

        GLuint _vbo_indices;
        glGenBuffers(1, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(unsigned int), &indexArray[0], GL_STATIC_DRAW);
        check_error_gl();

        numVertices = (unsigned) indexArray.size();

        glBindVertexArray(0);
    }

    void loadNormals(const std::vector<OpenGP::Vec3> &normalArray) {
        ///--- Vertex one vertex Array
        glBindVertexArray(_vao);
        check_error_gl();

        glGenBuffers(1, &_vnormal);
        glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
        glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(OpenGP::Vec3), &normalArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3 /*vec3*/, GL_FLOAT, GL_TRUE /*NORMALIZE*/, 3*sizeof(float) /*STRIDE*/, (void*)0 /*ZERO_BUFFER_OFFSET*/);
        check_error_gl();

        hasNormals = true;
        glBindVertexArray(0);
    }

    void loadTexCoords(const std::vector<OpenGP::Vec2> &tCoordArray) {
        ///--- Vertex one vertex Array
        glBindVertexArray(_vao);
        check_error_gl();

        glGenBuffers(1, &_tcoord);
        glBindBuffer(GL_ARRAY_BUFFER, _tcoord);
        glBufferData(GL_ARRAY_BUFFER, tCoordArray.size() * sizeof(OpenGP::Vec3), &tCoordArray[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2 /*vec2*/, GL_FLOAT, GL_FALSE /*DONT_NORMALIZE*/, 2*sizeof(float) /*STRIDE*/, (void*)0 /*ZERO_BUFFER_OFFSET*/);
        check_error_gl();

        hasTexCoords = true;
        glBindVertexArray(0);
    }

    void loadTextures(const std::string filename) {
        // Used snippet from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
        std::vector<unsigned char> image; //the raw pixels
        unsigned width, height;
        //decode
        unsigned error = lodepng::decode(image, width, height, filename);
        //if there's an error, display it
        if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

        // unfortunately they are upside down...lets fix that
        unsigned char* row = new unsigned char[4*width];
        for(int i = 0; i < int(height)/2; ++i) {
            memcpy(row, &image[4*i*width], 4*width*sizeof(unsigned char));
            memcpy(&image[4*i*width], &image[image.size() - 4*(i+1)*width], 4*width*sizeof(unsigned char));
            memcpy(&image[image.size() - 4*(i+1)*width], row, 4*width*sizeof(unsigned char));
        }
        delete row;

        glBindVertexArray(_vao);
        check_error_gl();

        glUseProgram(_pid);

        GLuint tex_id = glGetUniformLocation(_pid, "texImage");
        check_error_gl();
        glUniform1i(tex_id, 0);
        check_error_gl();

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);

        check_error_gl();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        check_error_gl();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
        check_error_gl();

        hasTextures = true;
        glUseProgram(0);
        glBindVertexArray(0);
    }

    void draw(OpenGP::Mat4x4 Model, OpenGP::Mat4x4 View, OpenGP::Mat4x4 Projection){

        if(!numVertices) return;

        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);

        ///--- Use normals when shading
        if(hasNormals) {
            glUniform1i(glGetUniformLocation(_pid, "hasNormals"), 1);
        } else {
            glUniform1i(glGetUniformLocation(_pid, "hasNormals"), 0);
        }

        ///--- Use textures when shading
        if(hasTextures && hasTexCoords) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glUniform1i(glGetUniformLocation(_pid, "hasTextures"), 1);
        } else {
            glUniform1i(glGetUniformLocation(_pid, "hasTextures"), 0);
        }

        ///--- Set the MVP to vshader
        glUniformMatrix4fv(glGetUniformLocation(_pid, "MODEL"), 1, GL_FALSE, Model.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "VIEW"), 1, GL_FALSE, View.data());
        glUniformMatrix4fv(glGetUniformLocation(_pid, "PROJ"), 1, GL_FALSE, Projection.data());

        check_error_gl();
        ///--- Draw
        glDrawElements(GL_TRIANGLES, /*#vertices*/ numVertices,
                    GL_UNSIGNED_INT,
                    0 /*ZERO_BUFFER_OFFSET*/);
        check_error_gl();

        ///--- Clean up
        glBindVertexArray(0);
        glUseProgram(0);
    }
};
