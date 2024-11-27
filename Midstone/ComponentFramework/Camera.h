#pragma once
#include <Vector.h>
#include <Matrix.h>
#include <Quaternion.h>
#include <DQMath.h>
#include "Transform.h"
#include "SkyBox.h"
#include "Trackball.h"
#include "Constants.h"
using namespace MATH;
using namespace MATHEX;

class Camera {
private:
	
	Matrix4 projection;
	DualQuat viewDq;
	bool orbitMode = false;

	Quaternion orientation;
	Vec3 position;
	Matrix4 view;
	SkyBox* skybox;
	Trackball trackball;
	Transform transform;
public:
	
	Camera();
	~Camera();
	void SetView(const Quaternion& orientation_, const Vec3& position_);
	void SetView(const Transform t_);
	Matrix4 GetViewMatrix() const;
	Matrix4 GetProjectionMatrix() const;
	DualQuat GetViewDQuaternion() const;
	void toggleOrbitMode() {
		orbitMode = !orbitMode;
	}

	bool OnCreate();
	void OnDestroy();
	void RenderSkyBox() const;
	void HandelEvents(const SDL_Event& sdlEvent);
	GLuint GetSkyBoxTextureID() const {
		return skybox->GetTextureID();
	}
	Quaternion GetOrientation() const { return orientation; }
};

