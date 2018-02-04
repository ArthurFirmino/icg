#version 330 core
// When you edit these shaders, Clear CMake Configuration so they are copied to build folders

out vec4 FragColor;

uniform int hasNormals;
uniform int hasTextures;

void main() {
    FragColor = vec4(1);
}
