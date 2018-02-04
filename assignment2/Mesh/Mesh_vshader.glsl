#version 330 core
// When you edit these shaders, Clear CMake Configuration so they are copied to build folders

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

uniform int hasNormals;
uniform int hasTextures;

void main() {
    gl_Position = PROJ * VIEW * MODEL * vec4(aPos, 1.0f);
}
