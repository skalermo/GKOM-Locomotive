#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform sampler2D Texture0;  

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

void main()
{
    vec4 texColor = texture(Texture0, TexCoord); 
	if(texColor.a < 0.1)
        discard;

	// ambient
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;

	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
	vec3 result = (ambient + diffuse + specular) * vec3(texColor);
    color = vec4(result, 1.0);
}
