#version 330
out vec4 out_color;

smooth in vec3 frag_normal;
smooth in vec3 frag_pos;


uniform vec3 light_pos;
uniform vec3 light_color;
uniform float power;
uniform float shininess;
uniform float attenuation;

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
    vec3 light_dir = vec3(0);
    float intensity = power * attenuate( frag_pos, light_dir );

    vec3 norm = normalize(frag_normal);
    float cos_a = dot(norm, light_dir);
    cos_a = max(cos_a, 0.0);

    vec3 eye_dir = normalize(-frag_pos);
    vec3 half_angle = normalize(light_dir + eye_dir);
    float blinn = dot(norm, half_angle);
    blinn = max(blinn, 0);
    blinn = cos_a > 1.0e-5 ? blinn : 0.0;
    blinn = pow(blinn, shininess);

    vec3 diffuse = light_color * intensity * cos_a;
    vec3 specular = specular_color * intensity * blinn;
    out_color =  vec4(0.1) + vec4(diffuse,1) + vec4(specular,1);
}
