#include "Body.h"







Body::Body(Transform* tran, Vec3 vel_, Vec3 acc_, float mass_) {
	transform = tran;
	vel = vel_;
	accel = acc_;
	mass = mass_;

}

void Body::Update(const float deltaTime) {
	//refactor this if I can figure out references/shared/pointers (I was banging my head against teh wall for 2 days so I'm just going to do the return for the time being.
	Vec3 v = transform->getPos();
	

	transform->setPos(transform->getPos() + vel * deltaTime + 0.5f * accel * deltaTime * deltaTime);//sets new position using kinematics
	vel += accel * deltaTime;//update velocity


}



Body::~Body() {}


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

