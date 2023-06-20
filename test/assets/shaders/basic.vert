//basic.vert
#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 10) in mat4 M;
layout (location = 14) uniform mat4 V;
layout (location = 18) uniform mat4 P;

out vec2 frag_position;

void main(void){
    frag_position = uv;
    gl_Position = P * V * M * vec4(position, 1.0);
}