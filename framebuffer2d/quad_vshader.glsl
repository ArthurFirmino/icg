R"(
#version 330 core
in vec3 vposition;
in vec2 vtexcoord;
out vec2 uv;

uniform mat4 M;

void main() {
    gl_Position = M * vec4(vposition, 1.0);
    uv = vtexcoord;
}
)"
