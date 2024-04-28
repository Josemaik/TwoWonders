#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

uniform sampler2D texture0;
uniform vec4 customColor;

// Material properties
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

#define MAX_POINT_LIGHTS 10

// Light
struct PointLight {
    vec4 position;
    vec4 color;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointsLights[MAX_POINT_LIGHTS];
uniform int NumPointLights;

vec3 Phong(){
    vec3 n = normalize(Normal);
    vec3 v = normalize(vec3(-FragPos));
    vec3 totalLight = vec3(0.0);

    for(int i = 0; i < NumPointLights; i++){
        vec3 l = normalize(vec3(pointsLights[i].position) - FragPos);
        vec3 r = reflect(-l, n);

        float distance = length(pointsLights[i].position - vec4(FragPos, 1.0));
        float attenuation = 1.0 / (pointsLights[i].constant + pointsLights[i].linear * distance + pointsLights[i].quadratic * (distance * distance));

        vec3 ambientTerm = Ka * pointsLights[i].color.rgb;
        vec3 diffuseTerm = Kd * max(dot(l,n), 0.0) * pointsLights[i].color.rgb;
        vec3 specularTerm = Ks * pow(max(dot(r,v), 0.0), Shininess) * pointsLights[i].color.rgb;

        totalLight += attenuation * (ambientTerm + diffuseTerm + specularTerm);
    }

    return totalLight;
}

void main(){
   FragColor = texture(texture0, TextCoord) * customColor * vec4(Phong(), 1.0);
}