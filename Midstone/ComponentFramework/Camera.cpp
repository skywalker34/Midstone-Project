#include "Camera.h"
#include <MMath.h>

void Camera::SetView(const Quaternion& orientation_, const Vec3& position_) {
	transform.setOrientation(orientation_);
	transform.setPos(position_);
	SetView(transform);
	
}

void Camera::SetView(const Transform t_)
{
	transform = t_;
	DualQuat T = DQMath::translate(-transform.getPos());
	DualQuat R = DQMath::rotate(QMath::conjugate(transform.getOrientation()));
	if (orbitMode) {
		viewDq = T * R;
	}
	else {
		viewDq = R * T;
	}
	

}
	
Camera::Camera() {
	projection = MMath::perspective(45.0f, (16.0f / 9.0f), 1.0f, 100.0f);
	transform.setOrientation( Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)));
	transform.setPos(Vec3(0.0f, 0.0f, 0.0));
}

Camera::~Camera() {

}

Matrix4 Camera::GetProjectionMatrix() const {
	return projection;
}

DualQuat Camera::GetViewDQuaternion() const
{
	return DualQuat(viewDq);
}

Matrix4 Camera::GetViewMatrix()const {
	return DQMath::toMatrix4(viewDq);
}
