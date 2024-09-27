#include "Sphere.h"
#include "collision.h"

Sphere::Sphere() : transform{}, radius{} {
}

Sphere::Sphere(Vec3 transform_, float radius_)
{
	transform = transform_;
	radius = radius_;
}

Sphere::~Sphere() {}

void Sphere::Update(float deltaTime) {
}

bool Sphere::OnCreate() {
	return true;
}

void Sphere::OnDestroy() {
}

//void Sphere::Render() const {
//}
