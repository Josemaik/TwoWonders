#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TextCoord;
uniform sampler2D texture1;

uniform vec4 customColor;

void main()
{
   FragColor = customColor;
}