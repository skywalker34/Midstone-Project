#pragma once
#include "Ship.h"
#include "Bullet.h"

#include "Sphere.h"

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

	//reference to models
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
	std::vector<Bullet*> bullets;

	//Movement vectors
	Vec3 destination = Vec3(0, 0, 0);
	//rotation vectors
	Vec3 targetDirection = Vec3(0, 0, 0);
	Vec3 movingDirection;
	Vec3 initialDirection = FORWARD;
	Vec3 orbitAxis;
	//rotation values
	float slerpT = 0;
	float newAngle = 0;

	EnemyShip* closestEnemy; //used when deciding which potential enenmy is the closest
	EnemyShip* potentialTarget; //reference to an enemy ship that this ship could attack


	EnemyShip* activeTarget; //reference to the enemy this ship is attakcing

	
	bool isMoving = false; //whether the ship is moving in space or not
	bool isChasing = false; //whether the ship is chasing an enemy or not

	bool canFire = true; //whetehr or not the hsip can fir ethis frame

	float timeSinceShot = 0;//the time since the ship has last fired a shot
	float rateOfFire = 0.01; //how often this ship can shoot (seconds)

	
	const float projectileSpeed = 1.0;//the distance (units/frame) this ship's projectile moves
	const float collisionSphereRadius = 5.0f;//radius of the ship's collision

	Sphere detectionSphere; //sphere used to detect enemies in range

	//Targeting
	const float range = 50.0f; //radius in which the ship can shoot
	int currentTargetIndex = 0;//enenmy's index that the ship is shooting at
	bool wouldIntersectPlanet = false;//whether or not a path would intersect the planet
	bool isSwitchingTarget = false;//whetehr or not the ship is switching between targets or nor

	float currentTargetDistance; //how far teh target is from this ship
	float potentialTargetDistance;//how far a enenmy hsip that isn't being a target is from the ship

		
	bool HappenOnce = true;//used to trigger the rocket sound loop once


	/// <summary>
/// Default constructor
/// Initializes a friendly ship with default values.
/// </summary>
	FriendlyShip();

	/// <summary>
	/// Parameterized constructor
	/// Initializes a friendly ship with a specific model and bullet model.
	/// </summary>
	/// <param name="model_">Pointer to the model associated with the friendly ship.</param>
	/// <param name="bulletModel_">Pointer to the model used for the bullets fired by the ship.</param>
	FriendlyShip(Model* model_, Model* bulletModel_);

	/// <summary>
	/// Initializes the friendly ship and sets up necessary resources.
	/// </summary>
	/// <returns>True if creation was successful, false otherwise.</returns>
	bool OnCreate() override;

	/// <summary>
	/// Cleans up resources used by the friendly ship.
	/// </summary>
	void OnDestroy();

	/// <summary>
	/// Updates the friendly ship's state based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void Update(const float deltaTime);

	/// <summary>
	/// Renders the friendly ship.
	/// </summary>
	/// <param name="shader">The shader to use for rendering.</param>
	void Render(Shader* shader) const;

	/// <summary>
	/// Renders the bullets fired by the friendly ship.
	/// </summary>
	/// <param name="shader">The shader to use for rendering the bullets.</param>
	void RenderBullets(Shader* shader) const;

	/// <summary>
	/// Renders the range of the friendly ship.
	/// </summary>
	/// <param name="shader">The shader to use for rendering the range.</param>
	void RenderRange(Shader* shader) const;

	/// <summary>
	/// Fires a bullet from the friendly ship. the bullet is fired along the foward vector with random spread variation
	/// </summary>
	void Fire();

	/// <summary>
	/// Moves the friendly ship to the specified destination.
	/// </summary>
	/// <param name="destination">The destination to move to.</param>
	void MoveToDestination(Vec3 destination);

	/// <summary>
	/// Rotates the friendly ship to face the specified target direction.
	/// </summary>
	/// <param name="targetDirection">The direction to face.</param>
	void RotateTowardTarget(Vec3 targetDirection);

	/// <summary>
	/// Checks if the friendly ship has reached its destination.
	/// </summary>
	/// <returns>True if the ship has reached its destination, false otherwise.</returns>
	bool HasReachDestination();

	/// <summary>
	/// Finds the closest enemy to the friendly ship.
	/// </summary>
	/// <param name="enemy">Pointer to the enemy ship.</param>
	void FindClosestEnemy(EnemyShip* enemy);

	/// <summary>
	/// Checks for intersections with other objects at the initial position.
	/// </summary>
	/// <param name="initailPosition">The initial position to check for intersections.</param>
	void CheckIntersection(Vec3 initailPosition);

	/// <summary>
	/// Orbits the friendly ship around the specified axis.
	/// </summary>
	/// <param name="axis">The axis to orbit around.</param>
	void Orbit(Vec3 axis);

	/// <summary>
	/// Moves the friendly ship directly to its destination.
	/// </summary>
	void MoveDirectly();

	/// <summary>
	/// Gets the bullets fired by the friendly ship.
	/// </summary>
	/// <returns>A reference to the vector of bullets.</returns>
	std::vector<Bullet*>& getBullets() {
		return bullets;
	};

	/// <summary>
	/// Destructor
	/// Ensures that resources used by the friendly ship are cleaned up properly.
	/// </summary>
	~FriendlyShip();

};

