#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

layout (std140) uniform Matrices
{
	mat4 projection;  //base allignment of 16 4 times, each with a different alligned offset
	mat4 view;
};

//Group up all output values inside an interface
out VS_OUT
{
	vec2 texCoord;
	vec3 Normal;
	vec3 FragPos; //Position of fragment in world space
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vs_out.texCoord = aTexCoord;
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal; //this calculation deals with non-uniform scaling 
	//mulitply position of incoming vertex by the model matrix
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0)); //Position value in world space coordinates that can be used by the fragment shader
	//Increase point size the further away the camera is
	gl_PointSize = gl_Position.z;
}