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

//void COLLISION::SphereSphereCollisionResponse(Sphere* body1, Sphere* body2) {
//	std::cout << "We have impact baby or in this case... I SEE YOU!" << std::endl;
//}
