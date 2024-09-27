#include "Ship.h"

Ship::Ship(int health_, int damage_, float speed_) {
	health = health_;
	damage = damage_;
	speed = speed_;

}

bool Ship::OnCreate()
{

	return false;
}


Ship::~Ship() {}