#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H
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

using namespace MATH;

/// Forward declarations 
union SDL_Event;


class SceneMainMenu : public Scene {
private:
	


	Window* window; //window for rendering IMGUI /SDL




public:
	/// <summary>
/// Explicit default constructor
/// Initializes the main menu scene with default values.
/// </summary>
	explicit SceneMainMenu();

	/// <summary>
	/// Parameterized constructor
	/// Initializes the main menu scene with a specific window.
	/// </summary>
	/// <param name="window_">Pointer to the window associated with the scene.</param>
	SceneMainMenu(Window* window_);

	/// <summary>
	/// Virtual destructor
	/// Ensures that resources used by the main menu scene are cleaned up properly.
	/// </summary>
	virtual ~SceneMainMenu();

	/// <summary>
	/// Initializes the main menu scene and sets up necessary resources.
	/// </summary>
	/// <returns>True if creation was successful, false otherwise.</returns>
	virtual bool OnCreate() override;

	/// <summary>
	/// Cleans up resources used by the main menu scene.
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// Updates the main menu scene's state based on the elapsed time. Nothing here, just leaving it here in case we need it later
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	virtual void Update(const float deltaTime) override;

	/// <summary>
	/// Renders the main menu scene. Nothing here, just leaving it here in case we need it later
	/// </summary>
	virtual void Render() override;

	/// <summary>
	/// Renders ImGui elements for the main menu scene.
	/// </summary>
	virtual void RenderIMGUI() override;

	/// <summary>
	/// Handles events for the main menu scene.
	/// </summary>
	/// <param name="sdlEvent">The SDL event to handle.</param>
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

};


#endif // SCENEUI2_H #pragma once
