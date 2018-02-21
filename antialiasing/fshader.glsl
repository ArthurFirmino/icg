R"(
#version 330 core

uniform float resolution_h;
uniform float resolution_w;

in vec2 uv;
out vec3 color;

float sample_shape(vec2 uv) {

    vec2 c1 = vec2(-0.2, -0.2);
    vec2 c2 = vec2(0.3, 0.4);

    float r1 = 0.1;
    float r2 = 0.2;

    vec2 l = c2 - c1;
    float ln = length(l);
    vec2 lh = l / ln;

    vec2 d = c2 - uv;
    vec2 dp = d - lh * dot(lh, d);

    vec2 offset = lh * length(dp) * (r2 - r1) / ln;

    float t = dot(uv + dp + offset - c1, lh) / length(c2 - c1);
    t = max(min(t, 1), 0);

    vec2 c3 = (1 - t) * c1 + t * c2;
    float r3 = (1 - t) * r1 + t * r2;

    return min(sign(length(uv - c3) - r3) + 1, 1);

}

void main() {

    /// TODO: sample the implicit function using a grid
    /// of points instead of one point per pixel. Average
    /// the sum of these samples to obtain an antialiased
    /// edge.
    color = vec3(1,1,1) * sample_shape(uv);
}
)"
