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

#define PI 3.14159265

const float F0 = 0.8;
const float k = 0.2;
const vec3 specular_color = vec3(0.25, 0.25, 0.25);
void main()
{
    vec3 light_dir = vec3(0);
    vec3 eye_dir = normalize(-frag_pos);
    float intensity = power * attenuate( frag_pos, light_dir );

    vec3 norm = normalize(frag_normal);
    float cos_a = dot(norm, light_dir);
    cos_a = max(cos_a, 0.0);

    float roughness = 1.0f / shininess;

    float Rs = 0.0;
    if (cos_a > 0) {
        vec3 half_angle = normalize(light_dir + eye_dir);
        float ndoth = max(dot(norm, half_angle), 0);
        float ndotv = max(dot(norm, eye_dir), 0);
        float vdoth = max(dot(light_dir, half_angle), 0);

        float fresnel = F0 + (1.0 - F0)*pow(1 - vdoth, 5.0);

        float m_squared = roughness * roughness;
        float r1 = 1.0 / (4.0 * m_squared * pow(ndoth, 4.0));
        float r2 = (ndoth * ndoth - 1.0) / (m_squared * ndoth * ndoth);
        float D = r1 * exp(r2);

        float two_ndoth = 2.0 * ndoth;
        float g1 = (two_ndoth * ndotv) / vdoth;
        float g2 = (two_ndoth * cos_a) / vdoth;
        float G = min(1.0, min(g1, g2));

        Rs = (fresnel * D * G) / (4 * PI * cos_a * ndotv);
    }

    vec3 diffuse = light_color * intensity * cos_a;
    vec3 specular = specular_color * intensity * cos_a * (k + Rs * (1.0 - k));
    out_color =  vec4(0.1) + vec4(diffuse,1) + vec4(specular,1);
}
