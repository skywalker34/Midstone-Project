#include "Body.h"
#include "QMath.h"






Body::Body(Transform* tran, Vec3 vel_, Vec3 acc_, float mass_) {
	//set values
	transform = tran;
	vel = vel_;
	accel = acc_;
	mass = mass_;

}

void Body::Update(const float deltaTime) {

	transform->setPos(transform->getPos() + vel * deltaTime + 0.5f * accel * deltaTime * deltaTime);//sets new position using kinematics
	vel += accel * deltaTime;//update velocity

}





void Body::UpdateOrientation(float deltaTime)
{
	// Get the angle from the angular velocity vector
	float changeInAngleRadians = VMath::mag(angularVel) * deltaTime;
	// if the angle is zero, get outta here
	if (changeInAngleRadians < VERY_SMALL) {
		return;
	}
	// Get the axis of rotation
	Vec3 axisOfRotation = VMath::normalize(angularVel);
	// Make a rotation quaternion using the axis and angle
	// We could do the cos(theta/2) + axis *sin(theta/2) thing here
	// Or cheat and use Scott's method
	float changeInAngleDegrees = changeInAngleRadians * RADIANS_TO_DEGREES;
	Quaternion rotation = QMath::angleAxisRotation(changeInAngleDegrees, axisOfRotation);
	// Update the orientation by multiplying
	orientation = orientation * rotation;
	transform->setOrientation(QMath::normalize(orientation));
}

Body::~Body() {}


void Body::ApplyForce(Vec3 force) {
	//use newtons second law (rearranged) to applya  force
	accel = force / mass;
}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {
}


