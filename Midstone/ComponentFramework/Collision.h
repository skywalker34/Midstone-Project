#ifndef COLLISION_H
#define COLLISION_H
#include "Sphere.h"
#include "DQMath.h"

//From Game Physics
using namespace MATHEX;
using namespace MATH;

namespace COLLISION {
	// Checks if two spheres have collided and return true or false.
	bool SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2);
	// Checks if a line (defined as a dual quaternion) and a sphere has collided
	bool LineSphereCollisionDetected(const Sphere* body, const DualQuat line);
	//checks if a point (in space) and a sphere have collided
	bool SpherePointCollisionDetected(const Sphere* body, const Vec3 pos);

}

#endif
