//basic.frag
#version 430 core

in vec3 frag_position;
out vec4 fragment;

void main(void){
    fragment = vec4((frag_position.x+1)/2, (frag_position.y+1)/2,(frag_position.z+1)/2,1.0);
}