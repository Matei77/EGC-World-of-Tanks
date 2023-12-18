#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int hp;
uniform int max_hp;

vec3 deformVertex(vec3 vertex, int hp, int max_hp) {
    float t = 1.0 - float(hp) / float(max_hp);
    float randomFactor = (fract(sin(dot(vertex, vec3(1.5, 1.5, 1.5)))) - 0.5) * 2.0;
    float scalingFactor = 0.2;
    return vertex + scalingFactor * randomFactor * t;
}

void main()
{
    vec3 damaged_pos = deformVertex(v_position, hp, max_hp);
            
    gl_Position = projection * view * model * vec4(damaged_pos, 1.0);
}
