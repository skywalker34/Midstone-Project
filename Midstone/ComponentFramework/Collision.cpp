#include "Collision.h"
#include "VMath.h"

bool COLLISION::SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2) {
	bool result = false;
	float sumOfRadii = body1->r + body2->r; //get the sum of the radius of both spheres combined
	float distance = VMath::mag(body2->center - body1->center); //get distanc ebetween 2 spheres

	//if the distance is less then the sum of the radii then the spheres are colliding
	if (distance <= sumOfRadii)
	{
		result = true;
	}

	return result;
}

bool COLLISION::LineSphereCollisionDetected(const Sphere* body, const DualQuat line_)
{
	//dualquat-sphere intersection referenced from Hamish Todd https://substack.com/home/post/p-142600125 (thanks Umer)


	DualQuat line = DQMath::normalize(line_);

	Vec4 centrePoint = body->center;

	Flector M = centrePoint * line;
	Plane pM = M.plane;//grab the plane part of the flector
	Vec4 vM = M.point;

	float radius = body->r;//hardcoded for now until sphere and collision classes are up
	float dSquared = (radius * radius) - (VMath::mag(vM) * VMath::mag(vM)); // d^2 = r^2 - (*<M>3)^2
	

	if (dSquared > 0) { //if d^2 > 0 then the line and sphere intersect
		return true;
	}

	return false;
}

bool COLLISION::SpherePointCollisionDetected(const Sphere* body, const Vec3 pos)
{
	//if the distance between a sphere and a point is less than the sphere's radius they're colliding
	//pretty straightforward
	float distance = VMath::mag(body->center - pos);
	if (distance < body->r) return true;
	return false;
}



