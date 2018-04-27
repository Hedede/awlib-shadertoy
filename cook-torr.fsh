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

float Fresnel_Schlick( float F0, float HoV )
{
        return F0 + (1.0 - F0)*pow(1 - HoV, 5.0);
}

const float F0 = 0.8;
const float k = 0.2;
const vec3 specular_color = vec3(0.25, 0.25, 0.25);
void main()
{
    vec3 light_dir = vec3(0);
    vec3 V = normalize(-frag_pos);
    float intensity = power * attenuate( frag_pos, light_dir );

    vec3 norm = normalize(frag_normal);
    float N_L = max(dot(norm, light_dir), 0.0);

    float roughness = 1.0f / shininess;
    float Rs = 0.0;
    if (N_L > 0) {
        vec3 H = normalize(light_dir + V);
        float N_H = max(dot(norm, H), 0);
        float N_V = max(dot(norm, V), 0);
        float V_H = max(dot(light_dir, H), 0);
        float H_V = max(dot(H, V), 0);

        float F = Fresnel_Schlick( F0, H_V );

        float m_squared = roughness * roughness;
        float r1 = 1.0 / (4.0 * m_squared * pow(N_H, 4.0));
        float r2 = (N_H * N_H - 1.0) / (m_squared * N_H * N_H);
        float D = r1 * exp(r2);

        float G = (2.0 * N_H * min(N_V, N_L)) / V_H;
        G = min(1.0, G);

        Rs = (F * D * G) / (4 * PI * N_L * N_V);
    }

    vec3 diffuse = light_color * intensity * N_L;
    vec3 specular = specular_color * intensity * N_L * (k + Rs * (1.0 - k));
    out_color =  vec4(0.1) + vec4(diffuse,1) + vec4(specular,1);
}
