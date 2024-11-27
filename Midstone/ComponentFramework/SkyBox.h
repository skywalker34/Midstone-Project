#pragma once
#include <glew.h>
#include "Shader.h"
class Mesh;
class SkyBox {
private:
	GLuint textureID;
	const char* posx, * negx, * posy, * negy, * posz, * negz;
	Mesh* cubeMesh;
	Shader* shader;
public:
	SkyBox(const char* posx_, const char* negx_, const char* posy_, const char* negy_, const char* posz_, const char* negz_);
	~SkyBox();
	bool OnCreate();
	void OnDestroy();
	void Render() const;

	Shader* GetShader() const {
		return shader;
	}

	GLuint GetTextureID() const {
		return textureID;
	}

private:
	bool LoadImages();

};

