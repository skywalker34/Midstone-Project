#ifndef SCENE3G_H
#define SCENE3G_H
#include "Scene.h"
#include "Vector.h"
#include "PlayerController.h"
#include "Actor.h"
#include "FriendlyShip.h"
#include "EnemyShip.h"
#include <Matrix.h>
#include "Collision.h"
#include "Vector.h"
#include "Planet.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "EnemySpawner.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Mesh;
class Shader;
class Bullet;
class Model;

class Scene3g : public Scene {
private:

	Shader* shader;
	Shader* bulletShader;
	Shader* planetShader;
	Shader* friendlyShipShader;

	Model friendlyShipModel;
	Model enemyShipModel;
	Model bulletModel;
	Model sphereModel;
	Model planeModel;
	

	EnemySpawner enemySpawnPoint = EnemySpawner(200, 5);

	
	Vec3 lightPos = Vec3(0.0f, 75.0f, -150.0f);

	PlayerController playerController;
	Vec3 shipWaypoint;

	Planet planet;

	std::vector<FriendlyShip*> playerFleet;
	std::vector<EnemyShip*> enemyFleet;

	int activeShip = 0;
	int startingFleetSize = 8;
	int enemyIndex = 0;
	
	float timeElapsed;

	int score = 0;

	bool drawInWireMode;
	bool isGameRunning = true;

	Window* window;

public:
	explicit Scene3g();
	Scene3g(Window* window_);
	virtual ~Scene3g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	void SpawnEnemy(const float deltaTime);
	void SetActiveShip();
	void RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime);
	void UpdatePlayerFleet(const float deltaTime);
	void UpdateEnemyFleet(const float deltaTime);
	void createModels();
	void createActors();
	void createShaders();
	void createClickGrid();
};


#endif // SCENE0_H#pragma once
