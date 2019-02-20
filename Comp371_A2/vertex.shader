
#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out fragmentPosition;
out vec3 norm;

void main()
{
    norm = mat3(transpose(inverse(Model))) * normals;
    gl_Position = Projection * View * Model * vec4(position.x, position.y, position.z, 1.0);
}
