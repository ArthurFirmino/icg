R"(
#version 330 core
uniform sampler2D noiseTex;

in vec2 uv;

out vec4 color;

void main() {
    // Texture size in pixels
    ivec2 size = textureSize(noiseTex, 0);

    /// TODO: Calculate surface normal N
    /// HINT: Use textureOffset(,,) to read height at uv + pixelwise offset
    vec3 A = vec3( 0 );
    vec3 B = vec3( 0 );
    vec3 C = vec3( 0 );
    vec3 D = vec3( 0 );
    vec3 N = normalize( cross(normalize(A-B), normalize(C-D)) );

    // Sample height from texture and normalize to [0,1]
    vec3 c = vec3((texture(noiseTex, uv).r + 1.0f)/2.0f);

    // (Optional): Visualize normals as RGB vector

    color = vec4(c,1);
}
)"
