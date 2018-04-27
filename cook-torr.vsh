#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 view_matrix;
uniform mat4 camtrafo;
uniform mat4 transform;

void main()
{
    WorldPos = vec3(transform * vec4(aPos, 1.0));
    Normal = mat3(transform) * aNormal;

    gl_Position =  view_matrix * camtrafo * vec4(WorldPos, 1.0);
}
