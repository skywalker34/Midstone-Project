#ifndef SCENE_H
#define SCENE_H
#include "irrKlang.h"

using namespace irrklang;

union SDL_Event;
class Scene  {
public:	
	/// <summary>
	/// Virtual destructor
	/// Ensures that derived classes clean up resources properly.
	/// </summary>
	virtual ~Scene() {}

	/// <summary>
	/// Pure virtual function for scene creation
	/// Must be overridden by derived classes to handle initialization logic.
	/// </summary>
	/// <returns>True if creation was successful, false otherwise.</returns>
	virtual bool OnCreate() = 0;

	/// <summary>
	/// Pure virtual function for scene destruction
	/// Must be overridden by derived classes to handle cleanup logic.
	/// </summary>
	virtual void OnDestroy() = 0;

	/// <summary>
	/// Pure virtual function to update the scene
	/// Must be overridden by derived classes to handle per-frame updates.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	virtual void Update(const float deltaTime) = 0;

	/// <summary>
	/// Pure virtual function to render the scene
	/// Must be overridden by derived classes to handle rendering logic.
	/// </summary>
	virtual void Render() = 0;

	/// <summary>
	/// Pure virtual function to render ImGui elements
	/// Must be overridden by derived classes to handle ImGui rendering.
	/// </summary>
	virtual void RenderIMGUI() = 0;

	/// <summary>
	/// Pure virtual function to handle events
	/// Must be overridden by derived classes to handle SDL events.
	/// </summary>
	/// <param name="sdlEvent">The SDL event to handle.</param>
	virtual void HandleEvents(const SDL_Event& sdlEvent) = 0;



	bool switchButton = false; //temp flag to allow scenes to communicate with the manager
	bool gameOverBool = false; //temp flag so when planet dies then GameOver screen occurs
	bool restartBool = false; //temp flag so when restart button in pause menu is pressed restart scene
	ISoundEngine* SoundEngine = createIrrKlangDevice(); // For Audio Music loop All scenes. not handled through audio manager as its just music
};
#endif