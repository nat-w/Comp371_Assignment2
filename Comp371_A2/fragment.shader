#version 330 core

out vec4 result;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

in vec3 fragment_position;
in vec3 norm;

void main()
{
    // ambient
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * light_color;

    // diffuse
    vec3 light_direction = normalize(light_position - fragment_position);
    float diffuse_strength = max(dot(normalize(norm), light_direction), 0.0f);
    vec3 diffuse = diffuse_strength * light_color;

    // specular
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(norm));
    float specular_strength = pow(max(dot(reflect_light_direction, view_direction), 0.0f), 32);
    vec3 specular = specular_strength * light_color;

    vec3 color = (specular + diffuse + ambient) * object_color;
    result = vec4(color, 1.0f);
} 
