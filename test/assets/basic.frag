#version 330

in vec3 frag_position;
out vec4 fragment;

void main(){
    fragment = vec4(frag_position, 1.0);
}