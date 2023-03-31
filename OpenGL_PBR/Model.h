#pragma once
#include <assimp/scene.h>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Model
{
public:

	Model();
	Model(const char* path);
	void Draw(Shader& shader);

private:
	//model data
	vector<Mesh> meshes;
	vector<MTexture> textures_loaded; //stores loaded textures 
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<MTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};

