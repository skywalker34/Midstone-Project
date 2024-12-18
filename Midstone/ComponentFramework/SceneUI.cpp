#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "SceneUI.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"
using namespace std;






SceneUI::SceneUI(SDL_Window* sdlWindow_)
{
	window = sdlWindow_;
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	screenRenderer = nullptr;

	
	backgroundTex = "textures/StartPhoto.jpg"; //Gets background photo

	backgroundTexture = nullptr;

}

bool SceneUI::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);

	

	//Create renderer for window
	screenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!screenRenderer)
	{
		std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initialize renderer color (black)
	SDL_SetRenderDrawColor(screenRenderer, 0, 0, 0, 255);

	// Initialize PNG image loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// Load the bodies's textures
	backgroundTexture = loadImage(backgroundTex);

	
	
	return true;
}

void SceneUI::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);

	SDL_DestroyTexture(backgroundTexture);

	backgroundTexture = nullptr;

	//Destroy  the renderer
	if (screenRenderer)
	{
		SDL_DestroyRenderer(screenRenderer);
		screenRenderer = nullptr;
	}


}
SceneUI::~SceneUI()
{
	delete background;
}
// ^^ Delete Anything to do with background when being destroyed 

void SceneUI::HandleEvents(const SDL_Event& sdlEvent) 
{
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_M:
			switchButton = true;
			cout << "pressed" << std::endl;
			break;
		}
	}
}


void SceneUI::Update(const float deltaTime)
{
	
		
}

void SceneUI::Render() {
	
	//Clear the Screen
	SDL_RenderClear(screenRenderer);

    //Render the background to the window.
	SDL_RenderCopy(screenRenderer, backgroundTexture, nullptr, nullptr);

	//Update the screen
	SDL_RenderPresent(screenRenderer);
}

SDL_Texture* SceneUI::loadImage(const char* textureFile)
{
	//Final Texture
	SDL_Texture* newTexture = nullptr;

	//Load Image at Specific Path
	SDL_Surface* loadedSurface = IMG_Load(textureFile);
	if (!loadedSurface)
	{
		cout << "Unable to load image " << textureFile <<
			"! SDL_IMAGE Error: " << IMG_GetError() << endl;
	}
	else 
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(screenRenderer, loadedSurface);
		if (!newTexture)
		{
			cout << "Unablee to create texture " << textureFile << "! SDL_ERROR: " << SDL_GetError() << endl;
		}
		//Get rid of old loaded Surface
		SDL_FreeSurface(loadedSurface);
	}
	
	return newTexture;
}

SDL_Rect SceneUI::scale(SDL_Texture* objectTexture, int start_x, int start_y, float scale)
{
	//Get the size of the input texture in pixels
	SDL_Point size;
	SDL_QueryTexture(objectTexture, nullptr, nullptr, &size.x, &size.y);
	SDL_Rect dest = { start_x, start_y, size.x * scale, size.y * scale };
	return dest;
}




