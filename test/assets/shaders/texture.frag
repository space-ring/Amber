//texture.frag
#version 430 core

in vec2 frag_position;
out vec4 fragment;

layout (location = 22) uniform sampler2D yellow;

void main(void){
    fragment = texture(yellow, frag_position);
}