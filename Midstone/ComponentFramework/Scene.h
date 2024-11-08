#ifndef SCENE_H
#define SCENE_H

union SDL_Event;
class Scene  {
public:	
	virtual ~Scene() {}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render()  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	bool switchButton = false; //temp flag to allow scenes to communicate with the manager
	bool gameOverBool = false; //temp flag so when planet dies then GameOver screen occurs
};
#endif