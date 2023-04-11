#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<MTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	//Initialize variables
	EBO = 0;
	VBO = 0;
	VAO = 0;

	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	/* 
		ADJUST THIS! REQUIRES SPECIFIC STRINGS TO BE SET 
		IN ORDER FOR IT TO WORK
	*/
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int opacityNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		//Get current texture unit to bind buffer to
		glActiveTexture(GL_TEXTURE0 + i);

		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			number = to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = to_string(specularNr++);
		}
		else if (name == "texture_opacity")
		{
			number = to_string(opacityNr++);
		}
		//Send texture over to fragment shader
		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	//reset active texture ready for next call
	glActiveTexture(GL_TEXTURE0);

	//draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	//Generate VAO, VBO and EBO ready for buffer data
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Bind vertices to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//Bind indices to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Bind data buffer to OpenGL
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	//vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	//Unbind VAO to stop accidental calls to buffer
	glBindVertexArray(0);

}
