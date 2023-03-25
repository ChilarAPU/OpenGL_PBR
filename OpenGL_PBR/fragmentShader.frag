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

struct Material
{
	vec3 ambient; 
	vec3 diffuse; 
	vec3 specular;
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

void main()
{
	//FragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), 0.5);

	//Ambient Lighting
	vec3 ambient = light.ambient * material.ambient;

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); //Get angle difference between the position of the light and the current fragment
	float diff = max(dot(norm, lightDir), 0.0); //Use max() to ensure this value never returns below 0
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//reflection vector from the players view
	vec3 viewDir = normalize(viewPos - FragPos); //angle difference between fragment position and player position
	vec3 reflectDir = reflect(-lightDir, norm);
	//calculating specular 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //pow() represents the shininess of the specular
	vec3 specular = light.specular * (spec * material.specular);

	//Combine lighting onto final fragment colour
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
