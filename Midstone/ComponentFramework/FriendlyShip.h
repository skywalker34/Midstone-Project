#pragma once
#include "Ship.h"
#include "Bullet.h"
#include "ShipController.h"
#include "Constants.h"
#include <vector>
#include <QMath.h>
#include <VMath.h>
class FriendlyShip : 
	public Ship
{
	public:

		//Inherited:
		// Transform transform
		// Model model
		//health
		//damage
		//speed
		//body
		//modelmatrix
		std::vector<Bullet> bullets;
		ShipController controller;
		Vec3 destination = Vec3(0, 0, 0);
		Vec3 moveDirection;
		Vec4 color = BLUE;
		bool isMoving = false;
		float newAngle = 0;

		Sphere* detectionSphere = nullptr;

		FriendlyShip();
		bool OnCreate() override; //probably a way to do this without ovveriding
								//problem I was having is that ship inherits from actor so actor On creates model but the mesh is loaded in ship
								//might be a solution with filepaths (leaving this here so I don't forget)
		void OnDestroy();
		void Update(const float deltaTime);
		void Render(Shader* shader) const;
		bool wouldIntersectPlanet = false;
		void moveToDestination(Vec3 destination);
		bool hasReachDestination();

		~FriendlyShip();
};

