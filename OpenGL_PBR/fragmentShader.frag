#version 460
out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material
{
	sampler2D diffuse; 
	sampler2D specular;
	sampler2D emissive;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	//FragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), 0.5);

	//Ambient Lighting
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb; //Get the current fragment colour from the texture coordinate

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); //Get angle difference between the position of the light and the current fragment
	float diff = max(dot(norm, lightDir), 0.0); //Use max() to ensure this value never returns below 0
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb; 

	//reflection vector from the players view
	vec3 viewDir = normalize(viewPos - FragPos); //angle difference between fragment position and player position
	vec3 reflectDir = reflect(-lightDir, norm);
	//calculating specular 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //pow() represents the shininess of the specular
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	//implement emissive material
	vec3 emissive = vec3(0.0f, 0.0f, 0.0f);
	//if our specular map value is higher than 0, this means we are on a fragment with the wooden texture
	if (texture(material.specular, texCoord).rgb == 0) 
	{
		emissive = texture(material.emissive, texCoord).rgb;
	}

	//Combine lighting onto final fragment colour
	vec3 result = (ambient + diffuse + specular + emissive);
	FragColor = vec4(result, 1.0);
}
