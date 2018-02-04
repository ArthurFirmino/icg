#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;

out vec3 pos;
out vec3 normal;
out vec2 texUV;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

uniform int hasNormals;
uniform int hasTextures;

void main() {
    if(hasNormals>0) normal = aNorm;
    if(hasTextures>0) texUV = aTexCoord;
    pos = vec4(  MODEL * vec4(aPos, 1.0f) ).xyz;
    gl_Position = PROJ * VIEW * MODEL * vec4(aPos, 1.0f);
}
