#ifndef SCENEGAMEOVER_H
#define SCENEGAMEOVER_H
#include "Scene.h"
#include "Vector.h"
#include "PlayerController.h"
#include "Actor.h"
#include "FriendlyShip.h"
#include "EnemyShip.h"
#include <Matrix.h>
#include "Vector.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "LeaderBoard.h"

using namespace MATH;

using namespace std;

/// Forward declarations 
union SDL_Event;
class Mesh;
class Shader;

/// <summary>
/// Ending scene to display the leaderboard and let user know they've lost
/// </summary>
class SceneGameOver : public Scene {
private:
	
	
	

	Window* window; //window to render things to
	std::vector<Leaderboard> leaderboard; //leaderboard data

public:
	/// <summary>
	/// Explicit default constructor
	/// Initializes a game over scene with default values. don't use
	/// </summary>
	explicit SceneGameOver();

	/// <summary>
	/// Parameterized constructor
	/// Initializes a game over scene, passed a window fo r IMGUI ussage
	/// </summary>
	/// <param name="window_">Pointer to the window associated with the scene.</param>
	SceneGameOver(Window* window_);

	/// <summary>
	/// Virtual destructor
	/// Ensures that resources used by the game over scene are cleaned up properly.
	/// </summary>
	virtual ~SceneGameOver();

	/// <summary>
	/// Initializes the game over scene and sets up necessary resources.
	/// </summary>
	/// <returns>True if creation was successful, false otherwise.</returns>
	virtual bool OnCreate() override;

	/// <summary>
	/// Cleans up resources used by the game over scene.
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// Updates the game over scene's state based on the elapsed time. theres nothing in there, just leaving it here for fture use
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	virtual void Update(const float deltaTime) override;

	/// <summary>
	/// Renders the 3D elements of the game over scene. theres nothing in there, just leaving it here for fture use
	/// </summary>
	virtual void Render() override;

	/// <summary>
	/// Renders ImGui elements for the game over scene.
	/// </summary>
	virtual void RenderIMGUI() override;

	/// <summary>
	/// Handles events for the game over scene.
	/// </summary>
	/// <param name="sdlEvent">The SDL event to handle.</param>
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

};


#endif // SceneGameOver_H #pragma once
