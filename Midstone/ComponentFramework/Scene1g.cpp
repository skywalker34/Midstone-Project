#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene1g.h"
#include <MMath.h>
#include <QMath.h>
#include <VMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"

Scene1g::Scene1g() : shader{ nullptr }, mesh{ nullptr },
drawInWireMode{ true } {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene1g::~Scene1g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene1g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	

	mesh = new Mesh("meshes/Sphere.obj");
	mesh->OnCreate();

	enemyFleet.push_back(new EnemyShip());
	enemyFleet[0]->OnCreate();
	enemyFleet[0]->transform.setPos(enemySpawnPoint);
	
	
	for (int i = 0; i <= startingFleetSize; i++) {
		playerFleet.push_back(new FriendlyShip());
	}

	playerFleet.push_back(new FriendlyShip());
	for (FriendlyShip* ship : playerFleet) {
		ship->OnCreate();
		ship->transform.setPos(Vec3(0.0f, 0, 0));
	}
	//friendlyShip = FriendlyShip();
	
	
	

	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();
	

	printf("On Create finished!!!!!");
	return true;


}

void Scene1g::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);

	

	mesh->OnDestroy();
	delete mesh;

	

	for (FriendlyShip* ship : playerFleet) {
		ship->OnDestroy();
		delete ship;
	}

	for (EnemyShip* ship : enemyFleet) {
		ship->OnDestroy();
		delete ship;
	}

	shader->OnDestroy();
	delete shader;


}

void Scene1g::HandleEvents(const SDL_Event& sdlEvent) {


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
			if (activeShip != 0) activeShip -= 1;
			break;
		case SDL_SCANCODE_X:
			if(activeShip != startingFleetSize + 1) activeShip += 1;
			break;

		case SDL_SCANCODE_P:
			//allows us to pause and unpause time, whoah.
			isGameRunning = !isGameRunning;
			break;
		}

		break;


		//below is SUUUUUUPER temporary
		//will be in collision namespace once its up and running
		if (playerController.hasDQLine) {
			DualQuat line = playerController.getLine();
			
			line = DQMath::normalize(line);


			//loop through the spheres
			for (FriendlyShip* ship : playerFleet) {
				Vec4 centrePoint = ship->transform.getPos();
				Flector M = centrePoint * line;
				Plane pM = M.plane;//grab the plane part of the flector
				Vec4 vM = M.point;

				float dSquared = (2 * 2) - (VMath::mag(vM) * VMath::mag(vM)); // d^2 = r^2 - (*<M>3)^2
				//pretty sure I'm doing this part wrong (*<M>3)^2

				if (dSquared > 0) { //if d^2 > 0 then the line and sphere intersect
					Vec4 i1 = line ^ (M.plane + Plane(0, 0, 0, sqrt(dSquared))) * -1;
					i1 = i1 / i1.w;
					i1.print("Intersection Point 1");



					Vec4 i2 = line ^ (M.plane + Plane(0, 0, 0, -sqrt(dSquared))) * -1;
					i2 = i2 / i2.w;
					i2.print("Intersection Point 2");
				}
			
	
			}


			//centrePoint = VMath::normalize(centrePoint4); //if I normalize the centre point I get the wrong answer 

			



			

		}
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

void Scene1g::Update(const float deltaTime) {

	
	if (isGameRunning) {
		playerController.Update(deltaTime);
		

		if (playerController.has3DClick) {

			
			shipWaypoint = playerController.getClickPos();
			playerFleet[activeShip]->moveToDestination(shipWaypoint);
			
			
			
		}
		for (FriendlyShip* ship : playerFleet) {
			ship->Update(deltaTime);
			ship->shipModelMatrix = ship->transform.toModelMatrix();
			ship->color = BLUE;//all friendly ships are blue except the active ship
		}
		playerFleet[activeShip]->color = GREEN;	//temporary to turn the selected ship green

	}

	

}

void Scene1g::Render() const {
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
	glUniform4fv(shader->GetUniformID("meshColor"), 1, Vec4(0.2f, 0.2f, 0.2f, 0.2f));
	mesh->Render(GL_TRIANGLES);
	
	for (FriendlyShip* ship : playerFleet) {
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		ship->Render(shader);
	}

	for (EnemyShip* ship : enemyFleet) {
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		ship->Render(shader);
	}

	playerController.Render(shader);

	glUseProgram(0);
}




