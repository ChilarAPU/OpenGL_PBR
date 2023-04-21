#version 460
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec2 texCoord;
	vec3 Normal;
	vec3 FragPos; //Position of fragment in world space
} gm_in[];

out VS_OUT
{
	vec2 texCoord;
	vec3 Normal;
	vec3 FragPos;
} fs_out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	fs_out.Normal = gm_in[0].Normal;
	fs_out.texCoord = gm_in[0].texCoord;
	fs_out.FragPos = gm_in[0].FragPos;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	fs_out.Normal = gm_in[1].Normal;
	fs_out.texCoord = gm_in[1].texCoord;
	fs_out.FragPos = gm_in[1].FragPos;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	fs_out.Normal = gm_in[2].Normal;
	fs_out.texCoord = gm_in[2].texCoord;
	fs_out.FragPos = gm_in[2].FragPos;
	EmitVertex();

	EndPrimitive();
}