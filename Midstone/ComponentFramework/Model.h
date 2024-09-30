#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include <string>

class Model
{
private:
	std::string filePath = "meshes/";	//ensure this matches where our meshes are kept
	std::string fileName ;

public:
	Texture tex;
	Mesh* mesh;
	Shader* shader;
	
	std::string meshName;

	Model();
	Model(std::string meshName_);
	bool OnCreate();
	void OnDestroy();
};

