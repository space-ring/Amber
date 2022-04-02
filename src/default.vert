#version 430 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_colour;
layout (location = 2) in vec3 v_normal;

layout (location = 10) in mat4 M;

layout (location = 14) uniform mat4 V;
layout (location = 18) uniform mat4 P;

uniform sampler2D tex;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 tex_coords;

void main(void){
    mat4 N = transpose(inverse(V * M));
    frag_position = vec3(M * vec4(v_position, 1.0));
    frag_normal = normalize(vec3(N * vec4(v_normal, 1.0)));
    tex_coords = v_colour;

    gl_Position = P * V * vec4(frag_position, 1.0);
}