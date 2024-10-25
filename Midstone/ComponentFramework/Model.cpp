#include "Model.h"

Model::Model()
{
}

Model::Model(std::string meshName_)
{
	
	meshName =  meshName_;
	fileName = filePath + meshName;
	
}

bool Model::OnCreate()
{
	std::cout << std::endl << "Loading :" + fileName << std::endl;
	mesh = new Mesh(fileName.c_str());
	
	if(mesh->OnCreate() == false) return false;

	return true;
	
}

void Model::OnDestroy()
{
	mesh->OnDestroy();
	delete mesh;
	
	/*shader->OnDestroy();
	delete shader;*/
}
