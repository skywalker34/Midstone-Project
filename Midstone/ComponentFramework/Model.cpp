#include "Model.h"

Model::Model()
{
}

Model::Model(std::string meshName_)
{
	
	meshName =  meshName_;
	meshFileName = meshFilePath + meshName;
	
}

Model::Model(std::string meshName_, std::vector<std::string> textureNames_)
{
	meshName = meshName_;
	meshFileName = meshFilePath + meshName;

	textureNames = textureNames_;
}

bool Model::OnCreate()
{
	std::cout << std::endl << "Loading :" + meshFileName << std::endl;
	mesh = new Mesh(meshFileName.c_str());
	
	if(mesh->OnCreate() == false) return false;

	if (!textureNames.empty()) {	//check if we have textures to create
		for (int i = 0; i < textureNames.size(); i++) { //if so loop through them...
			textureFileNames.push_back(textureFilePath + textureNames[i]);
			textures.push_back(new Texture());
			if (textures[i]->LoadImage(textureFileNames[i].c_str()) == false) {
				std::cout << std::endl << "WE have a problem... texture failed" << std::endl;
			}

		}
	}


	return true;
	
}

void Model::OnDestroy()
{
	mesh->OnDestroy();
	delete mesh;
	
	for (Texture* texture : textures) {
		delete texture;
	}

	/*shader->OnDestroy();
	delete shader;*/
}
