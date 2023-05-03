#version 460
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor, 1.0);
}