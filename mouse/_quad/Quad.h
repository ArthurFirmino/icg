#pragma once
#include "icg_common.h"
#include "OpenGP/Eigen/Image.h"
#include "OpenGP/external/LodePNG/lodepng.cpp"

class Quad{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer
    GLuint _tex; ///< Texture ID
    
public:    
    void init(){
        ///--- Compile the shaders
        _pid = OpenGP::load_shaders("_quad/quad_vshader.glsl", "_quad/quad_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
     
        ///--- Vertex coordinates
        {
            const GLfloat vpoint[] = { /*V1*/ -1.0f, -1.0f, 0.0f, 
                                       /*V2*/ +1.0f, -1.0f, 0.0f, 
                                       /*V3*/ -1.0f, +1.0f, 0.0f,
                                       /*V4*/ +1.0f, +1.0f, 0.0f };        
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(_pid, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Texture coordinates
        {
            const GLfloat vtexcoord[] = { /*V1*/ 0.0f, 0.0f, 
                                          /*V2*/ 1.0f, 0.0f, 
                                          /*V3*/ 0.0f, 1.0f,
                                          /*V4*/ 1.0f, 1.0f}; 
            
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vtexcoord_id = glGetAttribLocation(_pid, "vtexcoord");
            glEnableVertexAttribArray(vtexcoord_id);
            glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Load texture

        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);

        // Used snippet from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
        std::vector<unsigned char> image; //the raw pixels
        unsigned width, height;
        //decode
        unsigned error = lodepng::decode(image, width, height, "_quad/quad_texture.png");
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, /*level*/ 0, GL_RGBA,
                     width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
                
        ///--- Texture uniforms
        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
        
        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
           
    void cleanup(){
        /// TODO
    }
    
    void draw(const mat4& model, const mat4& view, const mat4& projection){
        glUseProgram(_pid);
        glBindVertexArray(_vao);        
            ///--- Bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _tex);
            
            ///--- Setup MVP
            mat4 MVP = projection*view*model;
            GLuint MVP_id = glGetUniformLocation(_pid, "MVP");
            glUniformMatrix4fv(MVP_id, 1, GL_FALSE, MVP.data());
            
            ///--- Draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};
