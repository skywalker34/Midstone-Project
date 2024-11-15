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

class SceneGameOver : public Scene {
private:
	
	bool drawInWireMode;
	bool isGameRunning = true;

	Window* window;

	bool show_demo_window;

	std::vector<Leaderboard> leaderboard;

public:
	explicit SceneGameOver();
	SceneGameOver(Window* window_);
	virtual ~SceneGameOver();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SceneGameOver_H #pragma once
