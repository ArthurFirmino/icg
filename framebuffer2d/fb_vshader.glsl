R"(
#version 330 core
in vec3 vposition;
in vec2 vtexcoord;
out vec2 uv;

void main() {
    gl_Position = vec4(vposition, 1.0);
    uv = vtexcoord;
}
)"
