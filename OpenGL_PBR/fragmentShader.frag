#version 460
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//FragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), 0.5);

	//Ambient Lighting
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); //Get angle difference between the position of the light and the current fragment
	float diff = max(dot(norm, lightDir), 0.0); //Use max() to ensure this value never returns below 0
	vec3 diffuse = diff * lightColor;

	//specular lighting
	float specularStrength = 0.5f;
	//reflection vector from the players view
	vec3 viewDir = normalize(viewPos - FragPos); //angle difference between fragment position and player position
	vec3 reflectDir = reflect(-lightDir, norm);
	//calculating specular 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //pow() represents the shininess of the specular
	vec3 specular = specularStrength * spec * lightColor;

	//Combine lighting onto final fragment colour
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
