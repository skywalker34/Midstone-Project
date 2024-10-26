#pragma once
#include "Ship.h"
#include "Bullet.h"

#include "Sphere.h"
#include "ShipController.h"
#include "Constants.h"
#include <vector>
#include <QMath.h>
#include <VMath.h>
using namespace MATH;
using namespace MATHEX;

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
		std::vector<Bullet*> bullets;
		ShipController controller;
		Vec3 destination = Vec3(0, 0, 0);

		Vec3 targetDirection = Vec3(0, 0, 0);
		Vec3 movingDirection;
		Vec3 initialDirection = FORWARD;

		Vec4 color = BLUE;
		bool isMoving = false;
		bool canFire = true;
		
		float timeSinceShot = 0;//the time since the ship has last fired a shot
		float rateOfFire = 0.2; //how often this ship can shoot (seconds)

		float slerpT = 0;
		float newAngle = 0;
		const float projectileSpeed = 0.1;//the distance (units/frame) this ship's projectile moves
		float range = 5.0f;
		int currentTargetIndex = 0;

		Sphere detectionSphere;

		FriendlyShip();
		bool OnCreate() override; //probably a way to do this without ovveriding
								//problem I was having is that ship inherits from actor so actor On creates model but the mesh is loaded in ship
								//might be a solution with filepaths (leaving this here so I don't forget)
		void OnDestroy();
		void Update(const float deltaTime);
		void Render(Shader* shader) const;
		bool wouldIntersectPlanet = false;
		void Fire();
		void moveToDestination(Vec3 destination);
		void rotateTowardTarget(Vec3 target);
		bool hasReachDestination();
		void setTargetEnemy(Vec3 target);

		std::vector<Bullet*>& getBullets() {
			return bullets; 
		}

		~FriendlyShip();
};

