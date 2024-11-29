#pragma once
#include "Ship.h"

/// <summary>
/// enemy ship is an actor that simply finds its target and "suicide bombs" it
/// </summary>
class EnemyShip : 
	public Ship
{
	

	//variables/components
	//damage - how much damage the ship does on impact
	//target - where the ship is headed



	//inherited from actor
		//Transform transform;
		//Model model;
	//inherited from ship:
		//int health;
		//int damage;
		//float speed;
		//Body* body;
		//Matrix4 shipModelMatrix;
	Vec3 target; //position in world space where the enemy is going to attack
	

	public:
		Vec3 aimingPoint;//point on this ship where an attacking ship should aim
		bool deleteMe = false;//flag to tell the scene when to delete this ship
		int shipIndex; //unique index assigned to this ship, think of it like a serial number
		

		/// <summary>
	/// Default constructor
	/// Initializes an enemy ship with default values.
	/// </summary>
		EnemyShip();

		/// <summary>
		/// Parameterized constructor
		/// Initializes an enemy ship with a specific position, model, and health.
		/// </summary>
		/// <param name="pos">The position of the enemy ship.</param>
		/// <param name="model_">Pointer to the model associated with the enemy ship.</param>
		/// <param name="health_">The health of the enemy ship.</param>
		EnemyShip(Vec3 pos, Model* model_, int health_);

		/// <summary>
		/// Sets the target position for the enemy ship.
		/// </summary>
		/// <param name="target_">The target position vector.</param>
		void setTarget(Vec3 target_);

		/// <summary>
		/// Calculates and returns the direction vector from the enemy ship to its target.
		/// </summary>
		/// <returns>The direction vector to the target.</returns>
		Vec3 getTargetDirection();

		/// <summary>
		/// Initializes the enemy ship and sets up necessary resources.
		/// </summary>
		/// <returns>True if creation was successful, false otherwise.</returns>
		bool OnCreate() override;

		/// <summary>
		/// Cleans up resources used by the enemy ship.
		/// </summary>
		void OnDestroy();

		/// <summary>
		/// Sets the unique index of the enemy ship. note, the index does not match the index in the array
		/// </summary>
		/// <param name="index">The index to set.</param>
		void setIndex(int index);

		/// <summary>
		/// Updates the enemy ship's state based on the elapsed time.
		/// </summary>
		/// <param name="deltaTime">The time elapsed since the last update.</param>
		void Update(const float deltaTime);

		/// <summary>
		/// Renders the enemy ship using the given shader.
		/// </summary>
		/// <param name="shader">The shader to use for rendering.</param>
		void Render(Shader* shader) const;

		/// <summary>
		/// Called when the enemy ship is hit by a b.ullet
		/// </summary>
		void Hit();


		
};

