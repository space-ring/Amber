//basic.frag
#version 430 core

in vec2 frag_position;
out vec4 fragment;

void main(void){
    fragment = vec4(frag_position/2+0.5, 1.0, 1.0);
}