#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

// --------------- //
// --- Texture --- //
// --------------- //

uniform sampler2D texture0;
uniform vec4 customColor;

// ---------------- //
// --- Material --- //
// ---------------- //

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

const float MIN_SHININESS = 0.1;

// -------------- //
// --- Lights --- //
// -------------- //

const int MAX_POINT_LIGHTS = 10;

struct PointLight {
    vec4 position;
    vec4 color;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointsLights[MAX_POINT_LIGHTS];
uniform int NumPointLights;

const int MAX_DIRECTIONAL_LIGHTS = 2;

struct DirectionalLight {
    vec3 direction;
    vec4 color;
};

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int NumDirectionalLights;

// ------------- //
// --- Phong --- //
// ------------- //

vec3 Phong(){

    vec3 totalLight = vec3(0.0);

    // pointsLights

    vec3 n = normalize(Normal);
    vec3 v = normalize(vec3(-FragPos));

    float ShininessAux = max(Shininess, MIN_SHININESS);

    int pLightCount = min(NumPointLights, MAX_POINT_LIGHTS);
    for(int i = 0; i < pLightCount; i++){
        vec3 l = normalize(vec3(pointsLights[i].position) - FragPos);
        vec3 r = reflect(-l, n);

        float distance = length(pointsLights[i].position - vec4(FragPos, 1.0));
        float attenuation = 1.0 / (pointsLights[i].constant + pointsLights[i].linear * distance + pointsLights[i].quadratic * (distance * distance));

        vec3 ambientTerm = Ka * pointsLights[i].color.rgb;
        vec3 diffuseTerm = Kd * max(dot(l,n), 0.0) * pointsLights[i].color.rgb;
        vec3 specularTerm = Ks * pow(max(dot(r,v), 0.0), ShininessAux) * pointsLights[i].color.rgb;

        totalLight += attenuation * (ambientTerm + diffuseTerm + specularTerm);
    }

    // directionalLights

    int dLightCount = min(NumDirectionalLights, MAX_DIRECTIONAL_LIGHTS);
    for(int i = 0; i < dLightCount; i++){
        vec3 lightDir = normalize(-directionalLights[i].direction);

        float diff = max(dot(n, lightDir), 0.0);

        vec3 reflectDir = reflect(-lightDir, n);
        float spec = pow(max(dot(reflectDir, v), 0.0), ShininessAux);

        vec3 ambientTerm = Ka * directionalLights[i].color.rgb;
        vec3 diffuseTerm = Kd * diff * directionalLights[i].color.rgb;
        vec3 specularTerm = Ks * spec * directionalLights[i].color.rgb;

        totalLight += (ambientTerm + diffuseTerm + specularTerm);
    }

    return totalLight;
}   

// ------------ //
// --- Main --- //
// ------------ //

void main(){
   FragColor = texture(texture0, TextCoord) * customColor * vec4(Phong(), 1.0);
}