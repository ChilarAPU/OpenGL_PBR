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
	sampler2D opacity;
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material material;
uniform vec3 viewPos;

uniform sampler2D grassTexture; 
uniform bool bIsTransparent;

//uniform sampler2D texture_diffuse1;

// Give attenuation a default value as not every light caster uses it
vec3 calculatePhongLighting(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float attenuation = 1);
vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

	//Get normalized normal and direction of fragment from camera
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	//vec3 ambient;
	//vec3 diffuse;
	//vec3 specular;

	//Directional light
	vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);
	//Point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += calculatePointLight(pointLights[i], norm, FragPos, viewDir);
	}
	//SpotLights
	//result += calculateSpotLight(spotLight, norm, FragPos, viewDir);
	//FragColor = vec4(result, 1.0);
	if (!bIsTransparent)
	{
		FragColor = vec4(result, 1.0);
	}
	else
	{
		vec4 texColor = texture(material.opacity, texCoord);
		//if (texColor.a < 0.1)
		//{
			//discard;
		//}

		FragColor = texColor;
	}

	//FragColor = vec4(texture(material.diffuse, texCoord));
	
	//Transform depth testing back to linear curve
	//float ndc = gl_FragCoord.z * 2.0 - 1.0;
	//float linearDepth = (2.0 * 0.1 * 100) / (100 + 0.1 - ndc * (100 - 0.1));
	//float depth = linearDepth / 50.0;

	//FragColor = vec4(vec3(depth), 1.0);

}

vec3 calculatePhongLighting(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float attenuation = 1)
{
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, lightDir), 0.0), material.shininess);
	//combine diffuse, specular and ambient
	vec3 ambient = ambientColor * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = diffuseColor * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = specularColor * spec * vec3(texture(material.specular, texCoord));
	//multiply lighting by any passed through attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction); //normalize and invert incoming light direction
	
	return calculatePhongLighting(lightDir, normal, viewDir, light.ambient, light.diffuse, light.specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos); //Get direction based from position of light source and fragment position
	vec3 halfwayDir = normalize(lightDir + viewDir); //get halfway vector to blinn-phong

	//attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

	return calculatePhongLighting(halfwayDir, normal, viewDir, light.ambient, light.diffuse, light.specular, attenuation);
}


vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

	//Radius light source from source of spot light and gradually decrease from cutOff to outerCutOff
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	//emissive
	vec3 emissive = vec3(0.0f, 0.0f, 0.0f);
	//if our specular map value is higher than 0, this means we are on a fragment with the wooden texture
	if (texture(material.specular, texCoord).rgb == 0) 
	{
		emissive = texture(material.emissive, texCoord).rgb;
	}
	//Use attenuation and spotlight intensity with emissive texture to convey the look of a hidden image on the mesh
	emissive *= attenuation * intensity * 2;

	vec3 phongLighting = calculatePhongLighting(lightDir, normal, viewDir, light.ambient, light.diffuse, light.specular, attenuation);

	//Multiply phong lighting by the spotlight 
	phongLighting *= intensity;

	return (phongLighting + emissive);
}