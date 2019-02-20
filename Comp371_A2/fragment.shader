#version 330 core

out vec4 color;

uniform vec3 objectcolor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

in vec3 fragmentPosition;
in vec3 norm;

void main()
{
    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = light_color * ambient_strength;

    // diffuse
    vec3 normal = normalize(norm);
    vec3 light_direction = light_position - fragment_position;
    diffuse_strength = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = light_color * diffuse_strength;

    // specular
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 r_light_direction = reflect(-light_direction, normal);
    float specular_strength = pow(max(dot(r_light_direction view_direction), 0.0),-32);
    vec3 specular = light_color * specular_strength;

    vec3 result = (specular + diffuse + ambient) * object_color;
    color = vec4(result, 1.0);
} 
