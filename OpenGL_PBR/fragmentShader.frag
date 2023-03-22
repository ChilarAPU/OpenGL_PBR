#version 460
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	FragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), 0.5);
}
