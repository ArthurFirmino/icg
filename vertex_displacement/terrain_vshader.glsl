R"(
#version 330 core
in vec3 vposition;
in vec2 vtexcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;

void main() {
    uv = vtexcoord;
    // TODO: Calculate vertical displacement h given uv
    float h = 0;
    // TODO: Apply displacement to vposition

    // DO THIS STEP FIRST:
    // TODO: Multiply model, view, and projection matrices in the correct order
    gl_Position = vec4(vposition, 1.0);
}
)"
