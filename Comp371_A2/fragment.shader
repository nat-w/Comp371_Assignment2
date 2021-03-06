#version 330 core

out vec4 result;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

uniform int gourard;
uniform int lightOff;
uniform int normalColor;

in vec3 fragment_position;
in vec3 gourard_color;
in vec3 norm;

void main()
{
    vec3 color;

    // ambient
    float ambient_strength = 0.25f;
    vec3 ambient = ambient_strength * light_color;

    // diffuse
    vec3 light_direction = normalize(light_position - fragment_position);
    float diffuse_strength = max(dot(normalize(norm), light_direction), 0.0f);
    vec3 diffuse = diffuse_strength * light_color;

    // specular
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(norm));
    float specular_strength = pow(max(dot(reflect_light_direction, view_direction), 0.0f), 64);
    vec3 specular = specular_strength * light_color;

    if (gourard == 1) {
        color = gourard_color;
    }

    else if (lightOff == 1) {
        color = ambient * object_color;
    }

    else if (normalColor == 1) {
        color = norm;
    }

    else {
        color = (specular + diffuse + ambient) * object_color;
    }
    result = vec4(color, 1.0f);
} 
