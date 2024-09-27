#pragma once
#include "Ship.h"
class EnemyShip : Ship
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
};

