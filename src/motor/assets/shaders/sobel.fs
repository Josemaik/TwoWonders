#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

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

void main(){
    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0);
    float gx = 0.0;
    float gy = 0.0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            vec2 offset = vec2(i - 1, j - 1) * tex_offset;
            vec3 sample = texture(screenTexture, TexCoords + offset).rgb;
            gx += sample.r * sobelX[i][j];
            gy += sample.r * sobelY[i][j];
        }
    }  

    float edgeStrength = sqrt(gx * gx + gy * gy);
    vec3 edgeColor = vec3(edgeStrength);

    FragColor = vec4(edgeColor, 1.0);  
}