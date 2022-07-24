#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 10) in mat4 M;
layout (location = 14) uniform in mat4 V;
layout (location = 18) uniform in mat4 P;

out vec3 frag_position;

void main(){
    frag_position = position;
    gl_Position = P * V * M * vec4(position, 1.0);
}