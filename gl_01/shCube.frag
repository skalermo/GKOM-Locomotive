#version 330 core
in vec2 TexCoord;
out vec4 color;
  
uniform sampler2D Texture0;

void main()
{
	//color = vec4(0.6f, 0.2f, 0.3f, 1.0f);
    color = texture(Texture0, TexCoord);
}
