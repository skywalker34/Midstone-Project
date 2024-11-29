#include "Camera.h"
#include <MMath.h>

void Camera::SetView(const Quaternion& orientation_, const Vec3& position_) {
	//set the vars in the transform
	transform.setOrientation(orientation_);
	transform.setPos(position_);
	SetView(transform);


	
}

void Camera::SetView(const Transform t_)
{
	//sets the view matrix for the camera
	transform = t_;
	DualQuat T = DQMath::translate(-transform.getPos());
	DualQuat R = DQMath::rotate(QMath::conjugate(transform.getOrientation()));
	
	//orbit mode is strictly for texting, as its not really proper
	//but by flipping the matricies we can get teh camera to orbit around the origin (facing the origin)
	if (orbitMode) {
		viewDq = T * R;
	}
	else {
		viewDq = R * T;
	}
	

}
	
Camera::Camera() {
	//constrcut with default values
	projection = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 500.0f);
	transform.setOrientation( Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)));
	transform.setPos(Vec3(0.0f, 0.0f, 0.0));

}

void Camera::OnDestroy() {
	if (skybox) {
		skybox->OnDestroy();
		delete skybox;
	}
}

Camera::~Camera() {}

bool Camera::OnCreate() {
	//create the skybox
	skybox = new SkyBox("textures/simple_skyboxp/skybox_front.png",
		"textures/simple_skyboxp/skybox_back.png",
		"textures/simple_skyboxp/skybox_up.png",
		"textures/simple_skyboxp/skybox_down.png",
		"textures/simple_skyboxp/skybox_right.png",
		"textures/simple_skyboxp/skybox_left.png");

	
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
	return DualQuat(viewDq); //returns the veiw as a dual quat
}

Matrix4 Camera::GetViewMatrix()const 
{
	return DQMath::toMatrix4(viewDq); //returns the view as a matrix4
}



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
