#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "Shader.h"

using namespace std;
using namespace glm;

struct Vertex
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

struct MTexture
{
	unsigned int id;
	string type;
	string path; //store path of texture to compare with other queries of the same texture
};

class Mesh
{
public:
	//mesh data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<MTexture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<MTexture> textures);
	//Draw mesh to viewport
	void Draw(Shader& shader);
private:
	//render data
	unsigned int VAO, VBO, EBO;

	// Setup OpenGL buffers 
	void setupMesh();

};

