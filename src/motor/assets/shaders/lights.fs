#version 330 core

const int MAX_POINT_LIGHTS = 10;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

out vec4 FragColor;

// TEXTURE //

uniform sampler2D texture0;
uniform vec4 customColor;

// LIGHTS //

const float AMBIENT_INTENSITY = 1.0;
const float DIFFUSE_INTENSITY = 0.5;

struct BaseLight
{
    vec3 color;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct Attenuation
{
    float constant;
    float linear;
    float exp;
};

struct PointLight
{
    BaseLight base;
    vec3 localPos;
    Attenuation at;
};

// MATERIAL //

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

uniform DirectionalLight gDirectionalLight;
uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

// FUNCTIONS //

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * 
                        AMBIENT_INTENSITY * 
                        vec4(Ka, 1.0f);

    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normal, -lightDirection);

    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(light.color, 1.0f) * 
                       DIFFUSE_INTENSITY * 
                       vec4(Kd, 1.0f) *
                       diffuseFactor;
    }

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float specularFactor = dot(viewDir, reflectDir);

    if(specularFactor > 0)
    {
        specularFactor = pow(specularFactor, Shininess);
        specularColor = vec4(light.color, 1.0f) *
                        Shininess *
                        vec4(Ks, 1.0f) *
                        specularFactor;
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight(vec3 normal)
{
    return CalcLightInternal(gDirectionalLight.base, gDirectionalLight.direction, normal);
}

void main(){
    vec3 normal = normalize(Normal);
    vec4 totalLight = CalcDirectionalLight(normal);

    FragColor = texture(texture0, TextCoord) * customColor * totalLight;
}