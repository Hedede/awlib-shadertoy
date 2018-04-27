#version 330
layout(location = 0) in vec4 position;

uniform mat4 view_matrix;
uniform mat4 camtrafo;
uniform mat4 transform;

void main()
{
        gl_Position = view_matrix * camtrafo * transform * position;
}
