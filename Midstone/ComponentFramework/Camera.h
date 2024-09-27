#pragma once
#include <Vector.h>
#include <Matrix.h>
#include <Quaternion.h>
#include <DQMath.h>
#include "Transform.h"
using namespace MATH;
using namespace MATHEX;

class Camera {
private:
	
	Matrix4 projection;
	DualQuat viewDq;

public:
	Transform transform;//temp here (move back to private later)
	Camera();
	~Camera();
	void SetView(const Quaternion& orientation_, const Vec3& position_);
	void SetView(const Transform t_);
	Matrix4 GetViewMatrix() const;
	Matrix4 GetProjectionMatrix() const;
	DualQuat GetViewDQuaternion() const;

};

