#include "Body.h"


Body::Body(Transform tran, Vec3 vel_, Vec3 acc_, float mass_)
	: transform(tran), vel(vel_), accel(acc_), mass(mass_) {}

Body::~Body() {}

Transform Body::Update(const float deltaTime, Transform t) {
	//refactor this if I can figure out references/shared/pointers (I was banging my head against teh wall for 2 days so I'm just going to do the return for the time being.
	transform = t; //sets the bodies transform to the passed argument
	Vec3 v = transform.getPos(); //gets and temporarily stores the transform position
	
	transform.setPos(v + vel * deltaTime + 0.5f * accel * deltaTime * deltaTime);//sets new position using kinematics
	vel += accel * deltaTime;//update velocity

	return transform;
}

void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {
}

void Body::Render() const {
}

