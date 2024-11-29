#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vector.h"
#include <string>

/// <summary>
/// Combines a mesh and compatible textures into one class (used for easier flyweight design pattern)
/// </summary>
class Model
{
private:
	//prefixes for directories
	std::string meshFilePath = "meshes/";	//ensure this matches where our meshes are kept
	std::string textureFilePath = "textures/";	//ensure this matches where our textures are kept

	std::string meshFileName; //string of the meshe's entire file path
	std::vector<std::string> textureFileNames;  //list of the textures entire file path

	std::vector<Texture*> textures; //list of textures that are compatible with this mesh

	

	std::vector<std::string> textureNames; //the name of the texures in the files
	std::string meshName; //the name of the mesh in the files

public:
	Mesh* mesh; //pointer to the mesh itself


	/// <summary>
	/// Default constructor for the Model class. (DO NOT USE)
	/// </summary>
	Model();

	// Two constructors as not all meshes need textures (e.g., bullet)
	/// <summary>
	/// Constructor for the Model class with a specified mesh name.
	/// </summary>
	/// <param name="meshName_">The name of the mesh.</param>
	Model(std::string meshName_);

	/// <summary>
	/// Constructor for the Model class with specified mesh and texture names.
	/// </summary>
	/// <param name="meshName_">The name of the mesh.</param>
	/// <param name="textureNames_">A vector of texture names.</param>
	Model(std::string meshName_, std::vector<std::string> textureNames_);

	/// <summary>
	/// Initializes the necessary resources or settings when creating the object.
	/// </summary>
	/// <returns>Returns true if the creation is successful, otherwise false.</returns>
	bool OnCreate();

	/// <summary>
	/// Cleans up resources or performs any necessary actions before destroying the object.
	/// </summary>
	void OnDestroy();

	/// <summary>
	/// Binds the textures for rendering between the specified start and end indices.
	/// </summary>
	/// <param name="startIndex">The starting index of the textures to bind.</param>
	/// <param name="endIndex">The ending index of the textures to bind.</param>
	void BindTextures(int startIndex, int endIndex) {
		for (int i = startIndex; i <= endIndex; i++) {
			glActiveTexture(GL_TEXTURE0 + (i - startIndex));
			glBindTexture(GL_TEXTURE_2D, textures[i]->getTextureID());
		}
	}

	/// <summary>
	/// Unbinds all textures.
	/// </summary>
	void UnbindTextures() {
		// Could do the indices thing but I want to be safe
		for (int i = 0; i <= textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

};

