#version 330 core

uniform int code;

out vec4 color;

void main(){

    if (code < 0) {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        color = vec4(code/255.0, 0, 0, 1.0);
    }
}
