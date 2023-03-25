#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 vertexColor;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos; //Position of fragment in world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vertexColor =  aColor;
	texCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal; //this calculation deal with non-uniform scaling 
	//mulitply position of incoming vertex by the model matrix
	FragPos = vec3(model * vec4(aPos, 1.0));
}