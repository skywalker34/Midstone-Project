#ifndef SCENE1G_H
#define SCENE1G_H
#include "Scene.h"
#include "Vector.h"
#include "PlayerController.h"
#include "Actor.h"
#include "FriendlyShip.h"
#include <Matrix.h>
#include "Vector.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Mesh;
class Shader;

class Scene1g : public Scene {
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
	Quaternion newPosition;
	std::vector<FriendlyShip*> playerFleet;
	
	bool drawInWireMode;
	bool isGameRunning = true;

public:
	explicit Scene1g();
	virtual ~Scene1g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H#pragma once
