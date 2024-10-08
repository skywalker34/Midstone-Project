#include "PlayerController.h"
#include "Constants.h"
#include <SDL.h>
#include <DQMath.h>
#include <Plane.h>



bool PlayerController::OnCreate()
{
	//here for future improvements
	transform.setPos(Vec3(0, 0, 20));
	
	clickGrid = Actor(Transform(), Model("Plane.obj"));

	if (clickGrid.OnCreate() == false) return false;
	return true;
	
}

void PlayerController::handleEvents(const SDL_Event& sdlEvent)
{
	trackball.HandleEvents(sdlEvent);
	transform.setOrientation(trackball.getQuat()); //trackball so viewer can "turn their head"

	Vec3 direction = QMath::rotate(-FORWARD, transform.getOrientation()); //gets the direction the camera is facing
	Vec3 v;//temporary
	switch (sdlEvent.type) {
	case SDL_KEYDOWN: //code that allows the user to move the camera around
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			//transform.setPos(transform.getPos() + (-direction * speed));
			//^^^THIS should work, no idea why it doesn't

			//	below works *marginally* better (still jank af)
			v = transform.getPos();
			v += (VMath::normalize(direction) * CAMERA_SPEED);
			transform.setPos(v);
			break;

		case SDL_SCANCODE_S:
			//transform.setPos(transform.getPos() + (direction * speed));


			v = transform.getPos();
			v += (VMath::normalize(-direction) * CAMERA_SPEED);
			transform.setPos(v);
			break;

			//future for strafing movement
		case SDL_SCANCODE_A:


			break;

		case SDL_SCANCODE_D:

			break;

		case SDL_SCANCODE_O:
			//if player hits o reset them to the origin
			transform = Transform();
			break;
		case SDL_SCANCODE_E:
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
	camera.SetView(transform);
	clickGrid.transform.setOrientation(transform.getOrientation());
	Vec3 cameraToGrid = VMath::normalize(camera.transform.getPos() - get3DClickCoords(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)) * (CAMERA_TO_ORIGIN + planeDepth);
	Vec3 newCridPosition =camera.transform.getPos() - cameraToGrid;
	clickGrid.transform.setPos(newCridPosition);
}

void PlayerController::Render(Shader* shader) const
{
	clickGrid.Render(shader);
}

Vec3 PlayerController::getClickPos()
{
	has3DClick = false;
	return clickPos;
	
}

Vec3 PlayerController::get3DClickCoords(float sdl_X, float sdl_Y)
{
	//below referenced from physics(semester 2) week 14
	Vec4 sdlPosPixelSpace = Vec4(sdl_X, sdl_Y, 0, 1);


	Vec4 sdlPosNDCSpace = MMath::inverse(MMath::viewportNDC(SCREEN_WIDTH, SCREEN_HEIGHT)) * sdlPosPixelSpace;
	// Let's get the front of the NDC box
	sdlPosNDCSpace.z = -1.0f;

	Vec4 sdlPosCameraSpace = MMath::inverse(camera.GetProjectionMatrix()) * sdlPosNDCSpace;
	// Divide out the w component
	sdlPosCameraSpace = sdlPosCameraSpace / sdlPosCameraSpace.w;

	Vec4 sdlPosWorldSpace = MMath::inverse(MATHEX::DQMath::toMatrix4(camera.GetViewDQuaternion())) * sdlPosCameraSpace;

	// Make a line from the camera position to the mouse
	// Using the join of two points
	line = transform.getPos() & sdlPosWorldSpace;

	// Make a plane based off scrollwheel
	Plane plane = Plane(0, 0, 1, -(-planeDepth));

	// intersection point is the meet of line and plane
	Vec4 intersection = line ^ plane;

	// Divide out the w component
	intersection = intersection / intersection.w;

		

		
		


	return Vec3(intersection);
}