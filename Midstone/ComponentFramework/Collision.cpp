#include "Collision.h"
#include "VMath.h"

bool COLLISION::SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2) {

	//The stuff straight from Umer.

	bool result = false;
	// Step 1: Find the distance between the two bodies
	// Step 2: Compare the distance to the sum of the radii
	// If distance <= r1 + r2, then result = true
	float sumOfRadii = body1->radius + body2->radius;
	//Find distance between two Bodies
	float distance = VMath::mag(body2->transform - body1->transform); //float distance = sqrt(pow(body2->pos.x - body1->pos.x, 2)) + sqrt(pow(body2->pos.y - body1->pos.y, 2)) + sqrt(pow(body2->pos.z - body1->pos.z, 2));
	if (distance <= sumOfRadii)
	{
		result = true;
	}
	return result;
}

void COLLISION::SphereSphereCollisionResponse(Sphere* body1, Sphere* body2) {
	std::cout << "We have impact baby or in this case... I SEE YOU!" << std::endl;

	//Not needed but im gonna keep this here for good measure
	//// Following Umer's assignment
	//// Step 1, find the normal vector (vector from body1 to body2)
	//Vec3 normal; // Remember to normalize this
	//normal = VMath::normalize(body2->pos - body1->pos);
	//// Step 2, find the relative velocity
	//float relVel = VMath::dot((body1->vel - body2->vel), normal);
	//// Step 3, IMPLUSE BABY
	//float J = -(1 + 0.99) * relVel / (1 / body1->mass + 1 / body2->mass);
	//// Step 4, calculate new velocity
	//body1->vel += J * normal / body1->mass;
	//body2->vel -= J * normal / body2->mass;
}
