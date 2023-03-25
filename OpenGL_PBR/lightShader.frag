#version 460
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor, 1.0);
}