#ifndef SCENEUI2_H
#define SCENEUI2_H
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

class SceneUI2 : public Scene {
private:
	
	bool drawInWireMode;
	bool isGameRunning = true;

	Window* window;

	bool show_demo_window;

public:
	explicit SceneUI2();
	SceneUI2(Window* window_);
	virtual ~SceneUI2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENEUI2_H #pragma once
