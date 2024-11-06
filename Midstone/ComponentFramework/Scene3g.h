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
	Shader* gridShader;

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

	int activeShip = -1;
	int startingFleetSize = 8;
	int enemyIndex = 0;
	
	float timeElapsed;

	int score = 0;

	bool drawInWireMode;
	bool isGameRunning = true;
	bool isGivingOrders = false;

public:
	explicit Scene3g();
	virtual ~Scene3g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
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
	void DestroyEnenmy(int index);

	void GameOver();
};


#endif // SCENE0_H#pragma once
