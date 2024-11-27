#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vector.h"
#include <string>

class Model
{
private:
	std::string meshFilePath = "meshes/";	//ensure this matches where our meshes are kept
	std::string textureFilePath = "textures/";	//ensure this matches where our textures are kept

	std::string meshFileName;
	std::vector<std::string> textureFileNames;

public:
	std::vector<Texture*> textures; 
	Mesh* mesh;
	Shader* shader;
	

	std::vector<std::string> textureNames;
	std::string meshName;

	Model();
	//two constructors as not all meshes need textures(ie. bullet)
	Model(std::string meshName_);
	Model(std::string meshName_, std::vector<std::string> textureNames_);
	bool OnCreate();
	void OnDestroy();

	void BindTextures(int startIndex, int endIndex) {

		for (int i = startIndex; i <= endIndex; i++) {
			glActiveTexture(GL_TEXTURE0 + (i - startIndex));
			glBindTexture(GL_TEXTURE_2D, textures[i]->getTextureID());
		}
	}

	void UnbindTextures() {
		//could do the indicies thing but I want to be safe
		for (int i = 0; i <= textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
};

