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
	meshFileName = meshFilePath + meshName; //create the full filepath name for mesh.cpp

	textureNames = textureNames_;
}

bool Model::OnCreate()
{
	mesh = new Mesh(meshFileName.c_str()); //create the mesh on the heap
	
	if(mesh->OnCreate() == false) return false; //if the  mesh fails to load, the model fails to load

	if (!textureNames.empty()) {	//check if we have textures to create
		for (int i = 0; i < textureNames.size(); i++) { //if so loop through them...
			textureFileNames.push_back(textureFilePath + textureNames[i]); //add the full texture file path to th elist
			textures.push_back(new Texture()); //create the texture on the heap
			if (textures[i]->LoadImage(textureFileNames[i].c_str()) == false) { //use sdl to load the image to the texture
				std::cout << std::endl << "WE have a problem... texture failed" << std::endl;
			}

		}
	}
	//if nothing failed, let the scene know its good to go.
	return true;
	
}

void Model::OnDestroy()
{
	//delete the mesh
	mesh->OnDestroy();
	delete mesh;
	
	//delete all the textures
	for (Texture* texture : textures) {
		delete texture;
	}

	
}
