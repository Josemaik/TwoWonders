#version 330 core

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light light;
uniform vec3 viewPos;
uniform vec3 objectColor;

// Agregar las variables uniformes que faltan
uniform mat4 mvp;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModel;
uniform mat4 matNormal;
uniform vec4 colDiffuse;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

// Agregar las variables de atributos que faltan
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec2 vertexTexCoord2;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec4 vertexColor;

void main()
{
    // Propiedades del material
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    int shininess = 32;

    // Cálculo de la iluminación ambiental
    vec3 ambient = ambientStrength * light.color;

    // Cálculo de la iluminación difusa
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(light.position - vertexPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    // Cálculo de la iluminación especular
    vec3 viewDir = normalize(viewPos - vertexPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * light.color;

    // Combinar los resultados
    vec3 result = (ambient + diffuse + specular) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}