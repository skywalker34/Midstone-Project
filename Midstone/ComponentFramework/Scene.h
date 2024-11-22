#ifndef SCENE_H
#define SCENE_H
#include "irrKlang.h"

using namespace irrklang;

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
	bool restartBool = false; //temp flag so when restart button in pause menu is pressed restart scene
	ISoundEngine* SoundEngine = createIrrKlangDevice(); // For Audio Throughout All scenes
};
#endif