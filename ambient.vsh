#version 330
layout(location = 0) in vec4 position;

smooth out vec4 frag_color;

uniform mat4 view_matrix;
uniform mat4 camtrafo;
uniform mat4 transform;

void main()
{
        frag_color =  vec4(0.1);
        gl_Position = view_matrix * camtrafo * transform * position;
}
