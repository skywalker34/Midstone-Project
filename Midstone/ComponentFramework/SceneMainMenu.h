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
class Mesh;
class Shader;

class SceneMainMenu : public Scene {
private:
	
	bool drawInWireMode;
	bool isGameRunning = true;

	Window* window;

	bool show_demo_window;


public:
	explicit SceneMainMenu();
	SceneMainMenu(Window* window_);
	virtual ~SceneMainMenu();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() override;
	virtual void RenderIMGUI() override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENEUI2_H #pragma once