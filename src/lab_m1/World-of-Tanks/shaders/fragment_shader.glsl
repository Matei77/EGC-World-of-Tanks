#version 330

uniform vec3 object_color;
uniform int hp;
uniform int max_hp;
uniform float alpha;

// Output
layout(location = 0) out vec4 out_color;


void main()
{   
    vec3 damaged_color;
//    vec3 color = (object_color.r, object_color.g, object_color.b);

    damaged_color = object_color * pow(0.7f, max_hp - hp);
    
    out_color = vec4(damaged_color, alpha);
}



