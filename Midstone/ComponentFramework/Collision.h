#ifndef COLLISION_H
#define COLLISION_H
#include "Sphere.h"

//From Game Physics

// Instead of a class, let’s put all the collision equations in a namespace.
// I got this idea from the book "Beautiful C++: 30 Core Guidelines for Writing
// Clean, Safe, and Fast Code" 
namespace COLLISION {
	// Checks if two spheres have collided and return true or false.
	// Remember we are assuming that all Body objects are spheres. I know it’s weird, we’ll fix it next course
	// Notice the “const” keyword here. Scott was impressed that I remembered to do that.
	// Means we are promising to look at but not change the Body objects. We are just
	// detecting collisions after all
	bool SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2);

	// If a collision has occurred, we need to immediately change the velocity
	// of both spheres. We will definitely be changing the Body object’s so there are
	// no “const” keywords here
	void SphereSphereCollisionResponse(Sphere* body1, Sphere* body2);
}

#endif