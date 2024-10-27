#include "Collision.h"
#include "VMath.h"

bool COLLISION::SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2) {
	bool result = false;
	float sumOfRadii = body1->r + body2->r;
	float distance = VMath::mag(body2->center - body1->center);
	if (distance <= sumOfRadii)
	{
		result = true;
	}
	return result;
}

bool COLLISION::LineSphereCollisionDetected(const Sphere* body, const DualQuat line_)
{
	DualQuat line = DQMath::normalize(line_);

	Vec4 centrePoint = body->center;

	Flector M = centrePoint * line;
	Plane pM = M.plane;//grab the plane part of the flector
	Vec4 vM = M.point;

	float radius = body->r;//hardcoded for now until sphere and collision classes are up
	float dSquared = (radius * radius) - (VMath::mag(vM) * VMath::mag(vM)); // d^2 = r^2 - (*<M>3)^2
	

	if (dSquared > 0) { //if d^2 > 0 then the line and sphere intersect
		return true;

		/*Vec4 i1 = line ^ (M.plane + Plane(0, 0, 0, sqrt(dSquared))) * -1;
		i1 = i1 / i1.w;
		i1.print("Intersection Point 1");



		Vec4 i2 = line ^ (M.plane + Plane(0, 0, 0, -sqrt(dSquared))) * -1;
		i2 = i2 / i2.w;
		i2.print("Intersection Point 2");*/
	}

	return false;
}



//void COLLISION::SphereSphereCollisionResponse(Sphere* body1, Sphere* body2) {
//	std::cout << "We have impact baby or in this case... I SEE YOU!" << std::endl;
//}
