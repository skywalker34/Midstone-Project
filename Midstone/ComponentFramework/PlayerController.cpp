#include "PlayerController.h"
#include "Constants.h"
#include <SDL.h>
#include <DQMath.h>
#include <Plane.h>
#include "Collision.h"


PlayerController::PlayerController()
{
}



bool PlayerController::OnCreate()
{

	//move the player controller out
	transform.setPos(Vec3(STARTING_POS)); 
	
	
	//if any on creates fail, then the whole thing fails, return false
	if (camera.OnCreate() == false) return false;

	if (clickGrid.OnCreate() == false) return false;
	return true;
	
}

void PlayerController::OnDestroy()
{
	//only thing to destroy is teh grid
	clickGrid.OnDestroy();
}

void PlayerController::CreateGrid(Model* model_)
{
	Transform t; //temp default transform
	t.setScale(65, 65, 1); //set the grid scale to large so player never sees the edges
	//create the actor
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
			//move the camera foward
			v = transform.getPos();
			v += (VMath::normalize(-forwardVector) * CAMERA_SPEED);
			break;
		case SDL_SCANCODE_S:
			//move the camera aback
			v = transform.getPos();
			v += (VMath::normalize(forwardVector) * CAMERA_SPEED);
			break;
		case SDL_SCANCODE_A:
			//Move the camera left
			v = transform.getPos();
			v += (VMath::normalize(-rightVector) * CAMERA_SPEED);
			break;
		case SDL_SCANCODE_D:
			//Move the camera Right
			v = transform.getPos();
			v += (VMath::normalize(rightVector) * CAMERA_SPEED);
			break;
		case SDL_SCANCODE_Q:
			//Move Camera up
			v = transform.getPos();
			v += (VMath::normalize(upVector) * CAMERA_SPEED);
			break;
		case SDL_SCANCODE_E:
			//Move camera down
			v = transform.getPos();
			v += (VMath::normalize(-upVector) * CAMERA_SPEED);	
			break;
		}
		break;

		//check to make sure the player is staying within their sphereical bounds
		if (!COLLISION::SpherePointCollisionDetected(&innerBounds, v) && COLLISION::SpherePointCollisionDetected(&outerBounds, v)) transform.setPos(v);
		break;

		

	case SDL_MOUSEBUTTONDOWN:
	{
		clickPos = get3DClickCoords(sdlEvent.button.x, sdlEvent.button.y);
		switch (sdlEvent.button.button){
		case SDL_BUTTON_LEFT:
			has3DClick = true;
			break;
		case SDL_BUTTON_RIGHT:
			hasCanceledOrder = true;
			break;
		}
		break;
	}
	break;
	case SDL_MOUSEWHEEL:
		//plane depth increases if the mouse is wheeled up (by the amount the mouse is wheeled)
		//plane depth decreases if the mouse is wheeled down (by the amount the mouse is wheeled)
		planeDepth += sdlEvent.wheel.preciseY * scrollSpeed;
		if(planeDepth > PLANE_MAX_DEPTH || planeDepth < PLANE_MIN_DEPTH) planeDepth -= sdlEvent.wheel.preciseY * scrollSpeed; //check if the edit moves the plane outside of its bounds, if so revert the change
		
		break;
	}

	mouseHoverPos2D = Vec3(sdlEvent.button.x, sdlEvent.button.y, 0);

}

void PlayerController::Update(const float deltaTime)
{

	int x, y;
	SDL_GetMouseState(&x, &y);

	mouseHoverPos3D = get3DClickCoords(x, y); 

	camera.SetView(transform);
	clickGrid.transform.setOrientation(transform.getOrientation());
	clickGrid.transform.setPos(get3DClickCoords(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));

	

}

void PlayerController::Render(Shader* shader) const
{
	//renders the grid
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	clickGrid.model->BindTextures(0,0);
	clickGrid.Render(shader);
	clickGrid.model->UnbindTextures();
	glDisable(GL_BLEND);
}



Vec3 PlayerController::getClickPos()
{
	//tells whatever is listening that the player has clicked
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
    DualQuat line_ = Vec4(0, 0, 0, 1) & sdlPosCameraSpace;

    // Make a plane in camera space (depth along the z-axis)
    Plane planeCameraSpace = Plane(0, 0, 1, planeDepth);

    // Calculate intersection point in camera space
    Vec4 intersectionCameraSpace = line_ ^ planeCameraSpace;
    intersectionCameraSpace = intersectionCameraSpace / intersectionCameraSpace.w;

    // Transform the intersection point to world space
    Vec4 intersectionWorldSpace = MMath::inverse(MATHEX::DQMath::toMatrix4(camera.GetViewDQuaternion())) * intersectionCameraSpace;

	Vec4 sdlPosWorldSpace = MMath::inverse(MATHEX::DQMath::toMatrix4(camera.GetViewDQuaternion())) * sdlPosCameraSpace;

	// Make a line from the camera position to the mouse
	// Using the join of two points
	line_ = transform.getPos() & sdlPosWorldSpace;


    return Vec3(intersectionWorldSpace.x, intersectionWorldSpace.y, intersectionWorldSpace.z);
}

void PlayerController::calculateLine()
{
	Vec4 sdlPosPixelSpace = Vec4(mouseHoverPos2D.x, mouseHoverPos2D.y, 0, 1);


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

}

void PlayerController::setPlayerBounds(float innerDis, float outerDis)
{
	//set the inner and outer bounds the camera cna move to
	innerBounds = Sphere(ORIGIN, innerDis);
	outerBounds = Sphere(ORIGIN, outerDis);
}
