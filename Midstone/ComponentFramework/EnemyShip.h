#pragma once
#include "Ship.h"

class EnemyShip : 
	public Ship
{
	//enemy ship is an actor that simply finds its target and "suicide bombs" it

	//variables/components
	//damage - how much damage the ship does on impact
	//target - where the ship is headed

	// Functions:
	// Move toward center
	// FindTarget() - ship decides where it needs to go

	//inherited from actor
		//Transform transform;
		//Model model;
	//inherited from ship:
		//int health;
		//int damage;
		//float speed;
		//Body* body;
		//Matrix4 shipModelMatrix;
	Vec3 target; 
	
	public:
		bool deleteMe = false;//flag to tell the scene when to delete this ship
		int shipIndex;
		Vec3 aimingPoint;

		EnemyShip();
		EnemyShip(Vec3 pos, Model* model_);

		void setTarget(Vec3 target_);
		Vec3 getTargetDirection();

		bool OnCreate() override;
		void OnDestroy();
		void setIndex(int index);
		void Update(const float deltaTime);
		void Render(Shader* shader) const;
		//called when the enemy ship is hit by a projectile
		void Hit();

		
};

