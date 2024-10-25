#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene0g.h"
#include <MMath.h>
#include <QMath.h>
#include <VMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"

Scene0g::Scene0g() : shader{ nullptr }, mesh{ nullptr },
drawInWireMode{ false } {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0g::~Scene0g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);


	mesh = new Mesh("meshes/Sphere.obj");
	mesh->OnCreate();
	if (friendlyShip.OnCreate() == false) {
		std::cout << "ship failed we have a problem";
	}
	//friendlyShip->model.mesh = new Mesh("meshes/Cube.obj");
	//riendlyShip->model.mesh->OnCreate();
	//added a pointer to fix the double transforms
	friendlyShip.transform.setPos(Vec3(3.0f, 0, 0));

	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	playerController.OnCreate();

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();
	friendlyShip.shipModelMatrix = MMath::translate(Vec3(3.0f, 0, 0)) * MMath::scale(Vec3(0.02f, 0.02f, 0.02f));

	printf("On Create finished!!!!!");
	return true;


}

void Scene0g::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);



	mesh->OnDestroy();
	delete mesh;

	//friendlyShip.model.mesh->OnDestroy();
	//delete friendlyShip.model.mesh;

	shader->OnDestroy();
	delete shader;


}

void Scene0g::HandleEvents(const SDL_Event& sdlEvent) {


	playerController.handleEvents(sdlEvent);
	//basically whats happening here is that the player controller has a boolean flag thats basically saying "I have something to tell you scenemanager"
	//the scene knows to check for this flag and to recieve the message so the playercontroller does not need to have a reference to the scene
	//its basically just throwing out this variable and hoping something is listening


	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_L:
			drawInWireMode = !drawInWireMode;
			break;
		case SDL_SCANCODE_Z:

			//axis = VMath::cross(destination, actor.transform.getPos());
			//newPosition = QMath::angleAxisRotation(1.0f, axis);
			//actor.transform.setPos(QMath::rotate(actor.transform.getPos(), newPosition));
			break;
		case SDL_SCANCODE_X:

			//axis = VMath::cross(destination, actor.transform.getPos());
			//newPosition = QMath::angleAxisRotation(-1.0f, axis);
			//actor.transform.setPos(QMath::rotate(actor.transform.getPos(), newPosition));
			break;
		}
		break;


		/////////////////////**DO NOT PUT ANY CONTROLS THAT AREN'T RELEVANT TO THE SCENE IN THE SCENE**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//use player controller instead for player controls
		/*case SDL_MOUSEMOTION:
			break;

		case SDL_MOUSEBUTTONDOWN:
			break;

		case SDL_MOUSEBUTTONUP:
		break;

		default:
			break;*/
	}
}

void Scene0g::Update(const float deltaTime) {

	playerController.Update(deltaTime);

	if (playerController.has3DClick) {
		playerController.getClickPos().print("3D click at: ");
		shipWaypoint = playerController.getClickPos();
		friendlyShip.moveToDestination(shipWaypoint);
	}





}

void Scene0g::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	mesh->Render(GL_TRIANGLES);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, friendlyShip.shipModelMatrix);
	//friendlyShip.model.mesh->Render(GL_TRIANGLES);
	glUseProgram(0);
}


















//below is teh scene0g that destroys the universe, why don't ask me I have no idea



//#include <glew.h>
//#include <iostream>
//#include <SDL.h>
//#include "Scene0g.h"
//#include <MMath.h>
//#include <QMath.h>
//#include <VMath.h>
//#include "Debug.h"
//#include "Mesh.h"
//#include "Shader.h"
//#include "Body.h"
//
//
//Scene0g::Scene0g() : shader{ nullptr }, mesh{ nullptr },
//drawInWireMode{ true } {
//	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
//}
//
//Scene0g::~Scene0g() {
//	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
//}
//
//bool Scene0g::OnCreate() {
//	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
//
//	
//	
//	mesh = new Mesh("meshes/Sphere.obj");
//	mesh->OnCreate();
//	friendlyShip.model.mesh = new Mesh("meshes/Cube.obj");
//	friendlyShip.model.mesh->OnCreate();
//	friendlyShip = FriendlyShip(); //default constructed
//	//added a pointer to fix the double transforms
//	friendlyShip.transform.setPos(Vec3(3.0f, 0.0f, 0.0f));	
//
//	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
//	if (shader->OnCreate() == false) {
//		std::cout << "Shader failed ... we have a problem\n";
//	}
//
//	playerController.OnCreate();
//
//	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
//	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
//	modelMatrix.loadIdentity();
//	friendlyShip.shipModelMatrix = MMath::translate(Vec3(3.0f, 0, 0)) * MMath::scale(Vec3(0.2f,0.2f,0.2f));
//	return true;
//
//
//}
//
//void Scene0g::OnDestroy() {
//	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
//
//
//
//	mesh->OnDestroy();
//	delete mesh;
//
//	friendlyShip.model.mesh->OnDestroy();
//	delete friendlyShip.model.mesh;
//
//	shader->OnDestroy();
//	delete shader;
//
//	
//}
//
//void Scene0g::HandleEvents(const SDL_Event &sdlEvent) {
//
//
//	playerController.handleEvents(sdlEvent);
//
//	switch( sdlEvent.type ) {
//    case SDL_KEYDOWN:
//		switch (sdlEvent.key.keysym.scancode) {
//			case SDL_SCANCODE_L:
//				drawInWireMode = !drawInWireMode;
//				break;
//			case SDL_SCANCODE_Z:
//				friendlyShip.moveToDestination(destination);
//				//axis = VMath::cross(destination, actor.transform.getPos());
//				//newPosition = QMath::angleAxisRotation(1.0f, axis);
//				//actor.transform.setPos(QMath::rotate(actor.transform.getPos(), newPosition));
//				break;
//			case SDL_SCANCODE_X:
//
//				//axis = VMath::cross(destination, actor.transform.getPos());
//				//newPosition = QMath::angleAxisRotation(-1.0f, axis);
//				//actor.transform.setPos(QMath::rotate(actor.transform.getPos(), newPosition));
//				break;
//		}
//		break;
//
//
//	/////////////////////**DO NOT PUT ANY CONTROLS THAT AREN'T RELEVANT TO THE SCENE IN THE SCENE**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//	//use player controller instead for player controls
//	/*case SDL_MOUSEMOTION:          
//		break;
//
//	case SDL_MOUSEBUTTONDOWN:              
//		break; 
//
//	case SDL_MOUSEBUTTONUP:            
//	break;
//
//	default:
//		break;*/
//    }
//}
//
//void Scene0g::Update(const float deltaTime) {
//	if (true == false) {
//		friendlyShip.moveToDestination(destination);	///in the future this will only be called when a ship needs to move
//		friendlyShip.body->Update(deltaTime);	///eventually will go in the ship's update
//		friendlyShip.transform.setPos(friendlyShip.transform.getPos()); /// see line 113 comment (same here)
//		friendlyShip.shipModelMatrix = MMath::translate(friendlyShip.transform.getPos()) * MMath::scale(Vec3(0.2f, 0.2f, 0.2f)); ///this will also be in the ship class
//	}
//	//!friendlyShip.hasReachDestination()
//
//}
//
//void Scene0g::Render() const {
//	/// Set the background color then clear the screen
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	if(drawInWireMode){
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	}else{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	}
//	glUseProgram(shader->GetProgram());
//	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE,  playerController.camera.GetProjectionMatrix());
//	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
//	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
//	mesh->Render(GL_TRIANGLES);
//	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, friendlyShip.shipModelMatrix);
//	friendlyShip.model.mesh->Render(GL_TRIANGLES);
//	glUseProgram(0);
//}
//
//
//
//	
