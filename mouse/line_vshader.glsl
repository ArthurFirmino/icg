R"(
#version 330 core
in vec2 vposition;

void main() {
    gl_Position = vec4(vposition, 1.0, 1.0);
}
)"
