#version 330 core
// Input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;

// Output vertex attributes
out vec3 FragPos; 
out vec2 TextCoord;

// Uniform variables
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   // Transform vertex position to world coordinates
   FragPos = vec3(model * vec4(aPos, 1.0));
   
   // Pass texture coordinates to the fragment shader
   TextCoord = aTextCoord;

   // Transform vertex position to clip coordinates
   gl_Position = projection * view * model * vec4(aPos, 1.0f);
}