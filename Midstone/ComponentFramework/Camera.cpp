#include "Camera.h"
#include <MMath.h>

void Camera::SetView(const Quaternion& orientation_, const Vec3& position_) {
	transform.setOrientation(orientation_);
	transform.setPos(position_);
	SetView(transform);


	orientation = orientation_;
	position = position_;
	
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
	projection = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	transform.setOrientation( Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)));
	transform.setPos(Vec3(0.0f, 0.0f, 0.0));

	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
	position = Vec3(0.0f, 0.0f, -15.0f);
	view = MMath::toMatrix4(orientation) * MMath::translate(position);
}

void Camera::OnDestroy() {
	if (skybox) {
		skybox->OnDestroy();
		delete skybox;
	}
}

Camera::~Camera() {}

bool Camera::OnCreate() {
	skybox = new SkyBox("textures/skybox_front.png",
		"textures/skybox_back.png",
		"textures/skybox_up.png",
		"textures/skybox_down.png",
		"textures/skybox_right.png",
		"textures/skybox_left.png");

	if (skybox->OnCreate() == false) {
		return false;
	}
	return true;

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

//void Camera::HandelEvents(const SDL_Event& sdlEvent) {
//	trackball.HandleEvents(sdlEvent);
//	orientation = trackball.getQuat();
//}

void Camera::RenderSkyBox() const {
	if (skybox == nullptr) return;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(skybox->GetShader()->GetProgram());
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("projectionMatrix"), 1, GL_FALSE, projection);
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::toMatrix4(transform.getOrientation()));
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("modelMatrix"), 1, GL_FALSE, Matrix4());
	/// Here I has turned on the shader and set the matricies. The shader will remain in this state
	/// until I turn off the shader. In Skybox::Render, I will bind the textures, because that is where
	/// they are, then draw the cube.  
	skybox->Render();
	glUseProgram(0);
}
