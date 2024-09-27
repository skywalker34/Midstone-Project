#ifndef COLLISION_H
#define COLLISION_H
#include "Sphere.h"

//From Game Physics

// Instead of a class, let’s put all the collision equations in a namespace.
// I got this idea from the book "Beautiful C++: 30 Core Guidelines for Writing
// Clean, Safe, and Fast Code" 
namespace COLLISION {
	// Checks if two spheres have collided and return true or false.
	bool SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2);

	// If a collision has occurred do something
	//void SphereSphereCollisionResponse(Sphere* body1, Sphere* body2);
}

#endif
