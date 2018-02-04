#version 330 core
in vec3 pos;
in vec3 normal;
in vec2 texUV;

out vec4 FragColor;

uniform sampler2D texImage;
uniform vec3 camera;

uniform int hasNormals;
uniform int hasTextures;

void main() {
    vec3 lightDir = normalize( camera - pos );
    if(hasNormals>0) {
        if(hasTextures>0) {
            FragColor = max(0.2, dot(lightDir, normal))*texture(texImage, texUV);
        } else {
            FragColor = max(0.2, dot(lightDir, normal))*vec4(1);
        }
    } else if(hasTextures>0) {
        FragColor = texture(texImage, texUV);
    } else {
        FragColor = vec4(1);
    }
}
