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
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Mesh;
class Shader;

class Scene3g : public Scene {
private:

	Shader* shader;
	Mesh* mesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	PlayerController playerController;
	Vec3 destination = Vec3(0, 3.0f, 0);
	Vec3 axis;
	Vec3 shipWaypoint;
	Vec3 enemySpawnPoint = Vec3(5.0f, 5.0f, 0);
	Quaternion newPosition;
	std::vector<FriendlyShip*> playerFleet;
	std::vector<EnemyShip*> enemyFleet;

	int activeShip = 0;
	int startingFleetSize = 1;
	
	bool drawInWireMode;
	bool isGameRunning = true;

public:
	explicit Scene3g();
	virtual ~Scene3g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H#pragma once
