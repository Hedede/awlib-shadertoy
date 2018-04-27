#version 330
layout(location = 0) in vec4 position;
layout(location = 2) in vec3 normal;

smooth out vec4 frag_color;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform float power;
uniform float shininess;
uniform float attenuation;

uniform mat4 view_matrix;
uniform mat4 camtrafo;
uniform mat4 transform;

float attenuate(in vec3 frag_pos, out vec3 light_dir)
{
        vec3 diff =  light_pos - frag_pos;
        float dist_sqr = dot(diff, diff);
        light_dir = diff * inversesqrt(dist_sqr);

        return (1 / ( 1.0 + attenuation * (dist_sqr)));
}

const vec3 specular_color = vec3(0.25, 0.25, 0.25);
void main()
{
    mat4 modelview = camtrafo * transform;
    vec4 pos =  modelview * position;

    vec3 vert_pos = pos.xyz;
    vec3 norm = normalize(mat3(modelview) * normal);

    vec3 light_dir = vec3(0);
    float intensity = power * attenuate( vert_pos, light_dir );

    float cos_a = dot(norm, light_dir);
    cos_a = max(cos_a, 0.0);

    vec3 diffuse = light_color * intensity * cos_a;
    frag_color =  vec4(0.1) + vec4(diffuse,1);

    gl_Position  = view_matrix * pos;
}

