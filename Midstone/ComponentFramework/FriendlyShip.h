#pragma once
#include "Ship.h"
#include "Bullet.h"

#include "Sphere.h"
#include "ShipController.h"
#include "Constants.h"
#include <vector>
#include <QMath.h>
#include <VMath.h>
#include "EnemyShip.h"

using namespace MATH;
using namespace MATHEX;

class FriendlyShip :
	public Ship
{
public:


	Model* rangeSphere;
	Model* bulletModel;

	Transform rangeSphereT;
	bool displayRange = false;
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
	Vec3 orbitAxis;
	EnemyShip* closestEnemy; //used when deciding which potential enenmy is the closest
	EnemyShip* potentialTarget; //reference to an enemy ship that this ship could attack

	EnemyShip* activeTarget; //reference to the enemy this ship is attakcing

	
	bool isMoving = false; //whether the ship is moving in space or not
	bool isChasing = false; //whether the ship is chasing an enemy or not

	bool canFire = true; //whetehr or not the hsip can fir ethis frame

	float timeSinceShot = 0;//the time since the ship has last fired a shot
	float rateOfFire = 0.01; //how often this ship can shoot (seconds)

	float slerpT = 0;
	float newAngle = 0;
	const float projectileSpeed = 1.0;//the distance (units/frame) this ship's projectile moves
	const float collisionSphereRadius = 5.0f;

	const float range = 50.0f;
	int currentTargetIndex = 0;
	bool wouldIntersectPlanet = false;
	bool isSwitchingTarget = false;

	float currentTargetDistance;
	float potentialTargetDistance;

		Sphere detectionSphere;
		bool HappenOnce = true;


	FriendlyShip();
	FriendlyShip(Model* model_, Model* bulletModel_);
	bool OnCreate() override; 
	void OnDestroy();
	void Update(const float deltaTime);
	void Render(Shader* shader) const;
	void RenderBullets(Shader* shader) const;
	void RenderRange(Shader* shader) const;

	void Fire();
	void MoveToDestination(Vec3 destination);
	void RotateTowardTarget(Vec3 targetDirection);
	bool HasReachDestination();
	void FindClosestEnemy(EnemyShip* enemy);
	void CheckIntersection(Vec3 initailPosition);
	void Orbit(Vec3 axis);
	void MoveDirectly();

	std::vector<Bullet*>& getBullets() {
		return bullets;
	};

	~FriendlyShip();
};

