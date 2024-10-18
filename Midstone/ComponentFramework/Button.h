
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

class Button
{
public:
	SDL_Texture* buttonTexture;

	
	const char* buttonTex;
	
	SDL_Renderer* screenRenderer;

	SDL_Texture* loadImage(const char* textureFile);

	SDL_Rect scale(SDL_Texture* objectTexture,
		int start_x = 0, int start_y = 0, float scale = 1.0f);

	Button();

	//Pass in the x and y of the top left corner of the sprite
	Button(int x, int y);
	
	
	bool OnCreate();
	void Update(const float deltaTime);
	void Render(int x, int y);
	//void HandleEvents(const SDL_Event& sdlEvent);
	
};