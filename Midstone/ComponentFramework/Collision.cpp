#include "Collision.h"
#include "VMath.h"

bool COLLISION::SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2) {
	bool result = false;
	float sumOfRadii = body1->radius + body2->radius;
	float distance = VMath::mag(body2->transform - body1->transform);
	if (distance <= sumOfRadii)
	{
		result = true;
	}
	return result;
}

//void COLLISION::SphereSphereCollisionResponse(Sphere* body1, Sphere* body2) {
//	std::cout << "We have impact baby or in this case... I SEE YOU!" << std::endl;
//}
