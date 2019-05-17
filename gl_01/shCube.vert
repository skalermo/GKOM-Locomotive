#version 330 core

layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	FragPos = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = texCoord;
	Normal = mat3(transpose(inverse(model))) * normal;  
} 
