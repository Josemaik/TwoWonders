#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

uniform sampler2D texture0;
uniform vec4 customColor;

// MATERIAL //

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

// LIGHTS //

uniform bool activeLights;

const int MAX_POINT_LIGHTS = 30;
const int MAX_SPOT_LIGHTS  = 10;

struct BaseLight
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
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
    vec3 position;
    Attenuation attenuation;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float cutOff;
};

uniform DirectionalLight gDirectionalLight;

uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform int gNumSpotLights;
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

// FUNCTIONS //

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * 
                        light.ambientIntensity * 
                        vec4(Ka, 1.0f);

    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normal, -lightDirection);

    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(light.color, 1.0f) * 
                       light.diffuseIntensity * 
                       vec4(Kd, 1.0f) *
                       diffuseFactor;
    }

    // vec3 viewDir = normalize(-FragPos);
    // vec3 reflectDir = reflect(-lightDirection, normal);
    // float specularFactor = dot(viewDir, reflectDir);

    // if(specularFactor > 0)
    // {
    //     specularFactor = pow(specularFactor, Shininess);
    //     specularColor = vec4(light.color, 1.0f) *
    //                     Shininess *
    //                     vec4(Ks, 1.0f) *
    //                     specularFactor;
    // }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight(vec3 normal)
{
    return CalcLightInternal(gDirectionalLight.base, gDirectionalLight.direction, normal);
}

vec4 CalcPointLight(PointLight l, vec3 normal)
{
    vec3 lightDirection = FragPos - l.position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = CalcLightInternal(l.base, lightDirection, normal);
    float attenuation = l.attenuation.constant +
                        l.attenuation.linear * distance +
                        l.attenuation.exp * distance * distance;

    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight l, vec3 normal)
{
    float auxCutOff = cos(radians(l.cutOff));

    vec3 lightToPixel = normalize(FragPos - l.base.position);
    float spotFactor = dot(lightToPixel, l.direction);

    if(spotFactor > auxCutOff)
        return CalcPointLight(l.base, normal) * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - auxCutOff));
    else 
        return vec4(0.0);
}

vec4 Phong()
{
	vec3 normal = normalize(Normal);
	vec4 totalLight = CalcDirectionalLight(normal);

	for(int i = 0; i < gNumPointLights; i++)
		totalLight += CalcPointLight(gPointLights[i], normal);

	for(int i = 0; i < gNumSpotLights; i++)
		totalLight += CalcSpotLight(gSpotLights[i], normal);

	return totalLight;
}

// SHADER CARTOON //

uniform bool activeShaderCartoon;

uniform float depthThreshold;
uniform float normalThreshold;
uniform vec4 _outlineColor;

mat3 sobelX = mat3(
    -1.0, 0.0, 1.0,
    -2.0, 0.0, 2.0,
    -1.0, 0.0, 1.0
);

mat3 sobelY = mat3(
    -1.0, -2.0, -1.0,
     0.0,  0.0,  0.0,
     1.0,  2.0,  1.0
);

float CalcDepthGradient()
{
    float center = texture(texture0, TextCoord).a;
    float gx = 0.0;
    float gy = 0.0;
    vec2 tex_offset = 1.0 / textureSize(texture0, 0);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            vec2 offset = vec2(i-1, j-1) * tex_offset;
            float sample = texture(texture0, TextCoord + offset).a;
            gx += (sample - center) * sobelX[i][j];
            gy += (sample - center) * sobelY[i][j];
        }
    }

    float gradient = sqrt(gx * gx + gy * gy);

    return min(pow(gradient / depthThreshold, 2), 1.0);
}

float CalcNormalGradient()
{
    vec3 center =  texture(texture0, TextCoord).rgb;
    float gx = 0.0;
    float gy = 0.0;
    vec2 tex_offset = 1.0 / textureSize(texture0, 0);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            vec2 offset = vec2(i-1, j-1) * tex_offset;
            vec3 sample = texture(texture0, TextCoord + offset).rgb;
            gx += length(sample - center) * sobelX[i][j];
            gy += length(sample - center) * sobelY[i][j];
        }
    }

    float gradient = sqrt(gx * gx + gy * gy);

    return min(pow(gradient / normalThreshold, 2), 1.0);
}

vec4 ShaderCartoon()
{
    vec3 sceneColor = texture(texture0, TextCoord).rgb;

    float sobelDepth = CalcDepthGradient();
    float soberNormal = CalcNormalGradient();

    float sobelOutline = clamp(max(sobelDepth, soberNormal), 0.0, 1.0);
    vec3 outlineColor = mix(sceneColor, _outlineColor.rgb, sobelOutline);
    
    vec3 color = mix(sceneColor, outlineColor, sobelOutline);

    return vec4(color, 1.0);
}

void main(){

    vec4 textureShader = texture(texture0, TextCoord);
    if(activeShaderCartoon)
        textureShader = ShaderCartoon();

    if(activeLights)
		FragColor = textureShader * customColor * Phong();
	else
   		FragColor = textureShader * customColor;
}