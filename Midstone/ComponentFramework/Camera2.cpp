#include "Camera2.h"
#include <SDL.h>
#include <MMath.h>

Camera2::Camera2() {
	projection = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
	position = Vec3(0.0f, 0.0f, -15.0f);
	view = MMath::toMatrix4(orientation) * MMath::translate(position);

}

void Camera2::SetView(const Quaternion& orientation_, const Vec3& position_) {
	orientation = orientation_;
	position = position_;
}

void Camera2::OnDestroy() {
	if (skybox) {
		skybox->OnDestroy();
		delete skybox;
	}
}

Camera2::~Camera2() {}

bool Camera2::OnCreate() {
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

void Camera2::HandelEvents(const SDL_Event& sdlEvent) {
	trackball.HandleEvents(sdlEvent);
	orientation = trackball.getQuat();
}

/// Our PASS tutor didn't like the name Render since it can't render itself. 
/// I hope this meets with his approval
void Camera2::RenderSkyBox() const {
	if (skybox == nullptr) return;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(skybox->GetShader()->GetProgram());
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("projectionMatrix"), 1, GL_FALSE, projection);
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("viewMatrix"), 1, GL_FALSE, MMath::toMatrix4(orientation));
	glUniformMatrix4fv(skybox->GetShader()->GetUniformID("modelMatrix"), 1, GL_FALSE, Matrix4());
	/// Here I has turned on the shader and set the matricies. The shader will remain in this state
	/// until I turn off the shader. In Skybox::Render, I will bind the textures, because that is where
	/// they are, then draw the cube.  
	skybox->Render();
	glUseProgram(0);
}