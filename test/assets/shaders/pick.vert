//pick.vert
#version 430 core

layout (location = 0) in vec3 v_position;

layout (location = 10) in mat4 M;
layout (location = 14) uniform mat4 V;
layout (location = 18) uniform mat4 P;
layout (location = 19) uniform int colour_offset;

out flat vec3 colour;

void main() {
    gl_Position = P * V * M * vec4(v_position, 1.0);
    int c = gl_InstanceID+colour_offset;
    colour = vec3(((c >> 16) & 255)/255.0, ((c >> 8) & 255)/255.0, (c & 255)/255.0);

}
