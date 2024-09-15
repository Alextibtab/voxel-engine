#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform vec3 light_pos;
uniform vec3 view_position;
uniform vec3 light_color;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;

    // Calculate distance to light source
    float distance = length(light_pos - FragPos);
    
    // Calculate attenuation
    float attenuationRadius = 20.0; // Adjust this value to change the light's radius
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    attenuation *= smoothstep(attenuationRadius, 0.0, distance);

    // Combine results with attenuation
    vec3 result = (ambient + diffuse + specular) * Color * attenuation;
    FragColor = vec4(result, 1.0);
}
