#include "SkyBox.h"
#include "Mesh.h"
#include "Shader.h"
#include <SDL_image.h>

SkyBox::SkyBox(const char* posx_, const char* negx_, const char* posy_,
	const char* negy_, const char* posz_, const char* negz_) :
	textureID(0), cubeMesh(nullptr) {
	posx = posx_;
	posy = posy_;
	posz = posz_;
	negx = negx_;
	negy = negy_;
	negz = negz_;
	//the six sides of the cube
}

bool SkyBox::OnCreate() {
	//pretty much all of this code was just simple xeroxed from Scott

	//create the skyBOX mesh (yes its just a box)
	cubeMesh = new Mesh("meshes/Cube.obj");
	bool status = cubeMesh->OnCreate();
	if (status == false) return false;//if we've failed then the whole skybox has failed

	//load upthe textures
	status = LoadImages();
	if (status == false) return false;//if we've failed then the whole skybox has failed

	//create the skybox shader
	shader = new Shader("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
	status = shader->OnCreate();

	if (status == false) return false; //if we've failed then the whole skybox has failed
	return true;
}

void SkyBox::OnDestroy() {
	cubeMesh->OnDestroy();
	delete cubeMesh;
	shader->OnDestroy();
	delete shader;
	glDeleteTextures(1, &textureID);
}


bool SkyBox::LoadImages() {
	SDL_Surface* textureSurface;
	int mode;


	//loads the textures for each side of the 3d sampler (skybox)
	//brute force method
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	textureSurface = IMG_Load(posx);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	textureSurface = IMG_Load(posy);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	textureSurface = IMG_Load(posz);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	textureSurface = IMG_Load(negx);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	textureSurface = IMG_Load(negy);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	textureSurface = IMG_Load(negz);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}

void SkyBox::Render() const {
	//render the skybox
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	cubeMesh->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

SkyBox::~SkyBox() {}