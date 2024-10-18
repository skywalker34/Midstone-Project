#include "Button.h"
#include <iostream>


Button::Button()
{
	buttonTex = "textures/StartImage.jpg"; // Gets button Image

	buttonTexture = nullptr;
}

Button::Button(int x, int y)
{

}
SDL_Texture* Button::loadImage(const char* textureFile)
{
	//Final Texture
	SDL_Texture* newTexture = nullptr;

	//Load Image at Specific Path
	SDL_Surface* loadedSurface = IMG_Load(textureFile);
	if (!loadedSurface)
	{
		std::cout << "Unable to load image " << textureFile <<
			"! SDL_IMAGE Error: " << IMG_GetError() << std::endl;
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(screenRenderer, loadedSurface);
		if (!newTexture)
		{
			std::cout << "Unablee to create texture " << textureFile << "! SDL_ERROR: " << SDL_GetError() << std::endl;
		}
		//Get rid of old loaded Surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

SDL_Rect Button::scale(SDL_Texture* objectTexture, int start_x, int start_y, float scale)
{	
	//Get the size of the input texture in pixels
	SDL_Point size;
	SDL_QueryTexture(objectTexture, nullptr, nullptr, &size.x, &size.y);
	SDL_Rect dest = { start_x, start_y, size.x * scale, size.y * scale };
	return dest;
}



bool Button::OnCreate()
{
	// Load the bodies's textures
	buttonTexture = loadImage(buttonTex);

	return true;
}

void Button::Update(const float deltaTime)
{
}

void Button::Render(int x, int y) 
{

	SDL_Rect buttons = scale(buttonTexture,x, y, 1.0f);
	SDL_RenderCopy(screenRenderer, buttonTexture, nullptr,&buttons);

	SDL_RenderPresent(screenRenderer);
}



