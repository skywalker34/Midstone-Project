#ifndef SCENEUI_H
#define SCENEUI_H
#include <SDL.h>
#include <SDL_image.h>
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;

class SceneUI : public Scene {
public:

	SDL_Window* window;
	int screenWidth, screenHeight, screenDepth = 1;
	float physicsScreenWidth = 30.0f, physicsScreenHeight = 15.0f, physicsScreenDepth = 0.0f;
	//float yPixel;
	//float xPixel;
	SDL_Renderer* screenRenderer;


	const char* backgroundTex;
	Body* background;

	SDL_Texture* backgroundTexture;

	SDL_Texture* loadImage(const char* textureFile);

	SDL_Rect scale(SDL_Texture* objectTexture,
		int start_x = 0, int start_y = 0, float scale = 1.0f);
	


	explicit SceneUI();
	SceneUI(SDL_Window* sdlWindow);
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	~SceneUI();

	bool switchButton = false; // IF button press switch scenes 
	
};


#endif // SCENEUI_H