R"(
#version 330 core
in vec3 vposition;
in vec2 vtexcoord;

void main() {
    gl_Position = vec4(vposition, 1.0);
}
)"
