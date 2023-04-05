#pragma once
#include <assimp/scene.h>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

struct TextureLoadReturn
{
	MTexture texture;
	bool bIsNewTexture;
};

class Model
{
public:

	Model();
	Model(const char* path);
	/* meshToDraw is to be used when only a certain mesh from a model wants to be drawn. Leave as default to render every mesh. 
	If the value given is too high, then default behaviour of drawing the whole mesh is used*/
	void Draw(Shader& shader, int meshToDraw = -1);

private:
	//model data
	vector<Mesh> meshes;
	vector<MTexture> textures_loaded; //stores loaded textures 
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<MTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	TextureLoadReturn loadTexture(aiTextureType type, string pathToTexture);

	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};

