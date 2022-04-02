#version 330 core

in vec3 frag_position;
in vec3 frag_normal;
in vec2 tex_coords;

layout (location = 22) uniform vec3 light_position;
layout (location = 23) uniform vec3 light_colour;
layout (location = 24) uniform float ambient;

uniform sampler2D tex;

out vec4 fragment;

void main(void){

    vec3 ambient_filter = ambient * light_colour;

    vec3 to_light = normalize(light_position - frag_position);
    float brightness = max(0.0, dot(frag_normal, to_light));
    vec3 diffuse_filter = brightness * light_colour;

    vec4 pixel = texture(tex, tex_coords);
    if (pixel.a < 1){
        discard;
    }
    vec4 sum_filter = vec4(max(ambient_filter, diffuse_filter), 1.0);
    fragment = pixel * sum_filter;
}