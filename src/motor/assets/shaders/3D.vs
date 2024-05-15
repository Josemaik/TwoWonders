#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

out vec3 FragPos; 
   // out vec3 Normal;
out vec2 TextCoord;
out vec3 tangents;
out vec3 bitangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0f);

   FragPos = vec3(model * vec4(aPos, 1.0));
      // Normal = mat3(transpose(inverse(model)) * aNormal);
   TextCoord = aTextCoord;
   tangents = aTangents;
   bitangents = aBitangents;
}