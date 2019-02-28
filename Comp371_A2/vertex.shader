#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform int gourard;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

out vec3 fragment_position;
out vec3 gourard_color;
out vec3 norm;

void main()
{
    if (gourard == 1) {
        // ambient
        float ambient_strength = 0.15f;
        vec3 ambient = ambient_strength * light_color;

        // diffuse
        vec3 light_direction = normalize(light_position - fragment_position);
        float diffuse_strength = max(dot(normalize(normals), light_direction), 0.0f);
        vec3 diffuse = diffuse_strength * light_color;

        // specular
        vec3 view_direction = normalize(view_position - fragment_position);
        vec3 reflect_light_direction = reflect(-light_direction, normalize(norm));
        float specular_strength = pow(max(dot(reflect_light_direction, view_direction), 0.0f), 64);
        vec3 specular = specular_strength * light_color;

        gourard_color = (specular + diffuse + ambient) * object_color;
    }

    norm = mat3(Model) * normals;
    fragment_position = mat3(Model) * position;
    gl_Position = Projection * View * Model * vec4(position.x, position.y, position.z, 1.0);
}
