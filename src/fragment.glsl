#version 330 core

in vec3 normal;
in vec3 frag_pos;

out vec4 color;

uniform vec3 object_colour;
uniform vec3 light_colour;
uniform vec3 light_pos;

void main() {
    float ambient_strength = 0.5;
    vec3 ambient = ambient_strength * light_colour;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_colour;

    vec3 output_colour = (ambient + diffuse) * object_colour;
    color = vec4(output_colour, 1.0);
}

