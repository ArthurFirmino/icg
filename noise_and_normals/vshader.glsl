R"(
#version 330 core
uniform sampler2D noiseTex;

in vec2 vposition;
in vec2 vtexcoord;

out vec2 uv;

void main() {
    uv = vtexcoord;
    gl_Position = vec4(vposition, 0.0, 1.0);
}
)"
