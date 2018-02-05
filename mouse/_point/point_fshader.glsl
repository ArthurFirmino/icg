#version 330 core

uniform bool selected;

out vec4 color;

void main(){
    if (selected)
        color = vec4(0.0, 1.0, 0.0, 1.0);
    else
        color = vec4(1.0, 0.0, 0.0, 1.0);
}
