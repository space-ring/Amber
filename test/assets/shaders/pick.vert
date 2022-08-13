//pick.vert
#version 430 core

layout (location = 0) in vec3 v_position;
layout (location = 3) in vec3 in_colour;
layout (location = 10) in mat4 M;
layout (location = 14) uniform mat4 V;
layout (location = 18) uniform mat4 P;

out vec3 colour;

void main() {
    gl_Position = P * V * M * vec4(v_position, 1.0);
    colour = in_colour;
}
