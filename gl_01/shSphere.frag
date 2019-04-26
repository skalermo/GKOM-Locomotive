#version 330 core
in vec3 ourPos;
in vec2 ourTextCoord;

out vec4 FragColor;  

uniform sampler2D ourTexture;
  
void main()
{
//	FragColor = vec4(0.5, 0.5, 0.5, 1.0);
	FragColor = texture(ourTexture, ourTextCoord);
}
