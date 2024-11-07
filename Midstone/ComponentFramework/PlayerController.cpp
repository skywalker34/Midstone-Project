#include "PlayerController.h"
#include "Constants.h"
#include <SDL.h>
#include <DQMath.h>
#include <Plane.h>



PlayerController::PlayerController()
{
}



bool PlayerController::OnCreate()
{
	//here for future improvements
	transform.setPos(Vec3(0, 0, 75));
	
	

	if (camera.OnCreate() == false) return false;

	if (clickGrid.OnCreate() == false) return false;
	return true;
	
}

void PlayerController::OnDestroy()
{
	clickGrid.OnDestroy();
}

void PlayerController::CreateGrid(Model* model_)
{
	Transform t;
	t.setScale(20, 20, 1);
	clickGrid = Actor(t, model_);
	clickGrid.OnCreate();
}


void PlayerController::handleEvents(const SDL_Event& sdlEvent)
{
	trackball.HandleEvents(sdlEvent);
	transform.setOrientation(trackball.getQuat()); //trackball so viewer can "turn their head"

	forwardVector = QMath::rotate(-FORWARD, transform.getOrientation()); //gets the direction the camera is facing

	
	forwardVector = VMath::normalize(forwardVector);

	// Calculate the left vector
	Vec3 rightVector = VMath::cross(UP, forwardVector);
	rightVector = VMath::normalize(rightVector);  // Normalize to get a unit vector

	// Calculate the up vector to ensure it's orthogonal to both forward and left
	Vec3 upVector = VMath::cross(forwardVector, rightVector);
	upVector = VMath::normalize(upVector);  // Normalize to get a unit vector


	Vec3 v;//temporary
	switch (sdlEvent.type) {
	case SDL_KEYDOWN: //code that allows the user to move the camera around
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			//transform.setPos(transform.getPos() + (-direction * speed));
			//^^^THIS should work, no idea why it doesn't

			//	below works *marginally* better (still jank af)
			v = transform.getPos();
			v += (VMath::normalize(-forwardVector) * CAMERA_SPEED);
			transform.setPos(v);
			break;

		case SDL_SCANCODE_S:
			//transform.setPos(transform.getPos() + (direction * speed));


			v = transform.getPos();
			v += (VMath::normalize(forwardVector) * CAMERA_SPEED);
			transform.setPos(v);
			break;

			//future for strafing movement
		case SDL_SCANCODE_A:

			v = transform.getPos();
			v += (VMath::normalize(-rightVector) * CAMERA_SPEED);
			transform.setPos(v);
			//move the camera right
			break;

		case SDL_SCANCODE_D:

			v = transform.getPos();
			v += (VMath::normalize(rightVector) * CAMERA_SPEED);
			transform.setPos(v);
			//move the camera left
			break;
		case SDL_SCANCODE_Q:

			v = transform.getPos();
			v += (VMath::normalize(upVector) * CAMERA_SPEED);
			transform.setPos(v);
			//move the camera UP
			break;
		case SDL_SCANCODE_E:

			v = transform.getPos();
			v += (VMath::normalize(-upVector) * CAMERA_SPEED);
			transform.setPos(v);
			//move the camera down
			break;

		case SDL_SCANCODE_O:
			//if player hits o reset them to the origin
			transform = Transform();
			if(planeDepth == 0); 
			{
				//push the plane into the screen a bit more so theres no divide by zero
				planeDepth += 1 ;
			}
			break;
		case SDL_SCANCODE_K:
			//switches between orbit and free mode
			camera.toggleOrbitMode();
			break;


		}
		break;

	case SDL_MOUSEBUTTONDOWN:
	{
		clickPos = get3DClickCoords(sdlEvent.button.x, sdlEvent.button.y);
		switch (sdlEvent.button.button){
		case SDL_BUTTON_LEFT:
			has3DClick = true;
			

			break;
		case SDL_BUTTON_RIGHT:

			Vec4 sdlPosPixelSpace = Vec4(sdlEvent.button.x, sdlEvent.button.y, 0, 1);


			Vec4 sdlPosNDCSpace = MMath::inverse(MMath::viewportNDC(SCREEN_WIDTH, SCREEN_HEIGHT)) * sdlPosPixelSpace;
			// Let's get the front of the NDC box
			sdlPosNDCSpace.z = -1.0f;

			Vec4 sdlPosCameraSpace = MMath::inverse(camera.GetProjectionMatrix()) * sdlPosNDCSpace;
			// Divide out the w component
			sdlPosCameraSpace = sdlPosCameraSpace / sdlPosCameraSpace.w;

			Vec4 sdlPosWorldSpace = MMath::inverse(MATHEX::DQMath::toMatrix4(camera.GetViewDQuaternion())) * sdlPosCameraSpace;

			// Make a line from the camera position to the mouse
			// Using the join of two points
			line2 = transform.getPos() & sdlPosWorldSpace;


			hasDQLine = true;

	
			break;
		}
		break;
	}
	break;

	case SDL_MOUSEBUTTONUP:
		break;

	case SDL_MOUSEWHEEL:
		planeDepth += sdlEvent.wheel.preciseY;
		//plane depth increases if the mouse is wheeled up (by the amount the mouse is wheeled)
		//plane depth decreases if the mouse is wheeled down (by the amount the mouse is wheeled)
		break;
	}

	

}

void PlayerController::Update(const float deltaTime)
{

	int x, y;
	SDL_GetMouseState(&x, &y);

	hoverPos = get3DClickCoords(x, y);


	camera.SetView(transform);
	clickGrid.transform.setOrientation(transform.getOrientation());

	/*Vec3 cameraToGrid = VMath::normalize(camera.transform.getPos() - get3DClickCoords(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)) * (planeDepth);
	Vec3 newCridPosition =camera.transform.getPos() - cameraToGrid;*/

	clickGrid.transform.setPos(get3DClickCoords(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

	

}

void PlayerController::Render(Shader* shader) const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	clickGrid.model->BindTextures(0,0);
	clickGrid.Render(shader);
	clickGrid.model->UnbindTextures();
	glDisable(GL_BLEND);
}



Vec3 PlayerController::getClickPos()
{
	has3DClick = false;
	return clickPos;
	
}

Vec3 PlayerController::get3DClickCoords(float sdl_X, float sdl_Y)
{
    // Below referenced from physics (semester 2) week 14
    Vec4 sdlPosPixelSpace = Vec4(sdl_X, sdl_Y, 0, 1);

    Vec4 sdlPosNDCSpace = MMath::inverse(MMath::viewportNDC(SCREEN_WIDTH, SCREEN_HEIGHT)) * sdlPosPixelSpace;
    // Let's get the front of the NDC box
    sdlPosNDCSpace.z = -1.0f;

    Vec4 sdlPosCameraSpace = MMath::inverse(camera.GetProjectionMatrix()) * sdlPosNDCSpace;
    // Divide out the w component
    sdlPosCameraSpace = sdlPosCameraSpace / sdlPosCameraSpace.w;

    // Make a line from the camera position to the mouse in camera space
    line = Vec4(0, 0, 0, 1) & sdlPosCameraSpace;

    // Make a plane in camera space (depth along the z-axis)
    Plane planeCameraSpace = Plane(0, 0, 1, -planeDepth);

    // Calculate intersection point in camera space
    Vec4 intersectionCameraSpace = line ^ planeCameraSpace;
    intersectionCameraSpace = intersectionCameraSpace / intersectionCameraSpace.w;

    // Transform the intersection point to world space
    Vec4 intersectionWorldSpace = MMath::inverse(MATHEX::DQMath::toMatrix4(camera.GetViewDQuaternion())) * intersectionCameraSpace;

	Vec4 sdlPosWorldSpace = MMath::inverse(MATHEX::DQMath::toMatrix4(camera.GetViewDQuaternion())) * sdlPosCameraSpace;

	// Make a line from the camera position to the mouse
	// Using the join of two points
	line = transform.getPos() & sdlPosWorldSpace;


    return Vec3(intersectionWorldSpace.x, intersectionWorldSpace.y, intersectionWorldSpace.z);
}
