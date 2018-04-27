#version 330
out vec4 out_color;
smooth in vec4 frag_color;

void main()
{
        out_color = max(frag_color, 0.1);
}
