#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture0;  

void main()
{
    vec4 texColor = texture(Texture0, TexCoord); 
	if(texColor.a < 0.1)
        discard;
    color = texColor;
}