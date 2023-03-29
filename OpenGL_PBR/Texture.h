#pragma once

#include <iostream>

using namespace std;

class Texture
{
private:
	unsigned int ID;

public:
	/* load texture image into ID variable*/
	Texture();

	void LoadTexture(const char* path);

	void BindTextureToBuffer(GLenum slot);
};

