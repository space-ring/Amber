//pick.frag
#version 430 core

in vec3 colour;
out vec4 fragment;

void main(void){
    fragment = vec4(colour, 1.0);
}