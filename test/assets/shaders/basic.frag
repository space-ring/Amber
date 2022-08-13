//basic.frag
#version 430 core

in vec3 frag_position;
out vec4 fragment;

void main(void){
    fragment = vec4(frag_position/2+0.5, 1.0);
}