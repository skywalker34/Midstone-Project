#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene3g.h"
#include <MMath.h>
#include <QMath.h>
#include <VMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Model.h"

#include <chrono>

Scene3g::Scene3g() : shader{ nullptr }, 
drawInWireMode{ true } {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene3g::~Scene3g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene3g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	
	


	///////////////////////////////////////////////////////////////MODELS\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	friendlyShipModel = Model("midstone_ship.obj");
	if (friendlyShipModel.OnCreate() == false) {
		printf("Model failed to load");
	}


	enemyShipModel = Model("enemyShip.obj");
	if (enemyShipModel.OnCreate() == false) {
		printf("Model failed to load");
	}

	bulletModel = Model("Bullet.obj");
	if (bulletModel.OnCreate() == false) {
		printf("Model failed to load");
	}

	sphereModel = Model("Sphere.obj", std::vector<std::string>{"planet_d.png",  "planet_s.png"});
	if (sphereModel.OnCreate() == false) {
		printf("Model failed to load");
	}
	

	planeModel = Model("Plane.obj");
	if (planeModel.OnCreate() == false) {
		printf("Model failed to load");
	}
	

	///////////////////////////////////////////////////ACTORS\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	for (int i = 0; i < startingFleetSize; i++) {
		enemyFleet.push_back(new EnemyShip(Vec3(5, 5, 0), &enemyShipModel));


	}
	for (EnemyShip* ship : enemyFleet) {
		ship->OnCreate();
		

	}
	
	
	for (int i = 0; i < startingFleetSize; i++) {
		playerFleet.push_back(new FriendlyShip(&friendlyShipModel, &bulletModel));
	}


	for (FriendlyShip* ship : playerFleet) {
		ship->OnCreate();
		ship->transform.setPos(Vec3(0.0f, 0, 0));
	}

	planet = Planet(30.0f, 5, &sphereModel, ORIGIN);
	planet.OnCreate();
	
	////////////////////////////////////////////////////////SHADERS\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	bulletShader = new Shader("shaders/bulletVert.glsl", "shaders/bulletFrag.glsl");
	if (bulletShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	planetShader = new Shader("shaders/planetVert.glsl", "shaders/planetFrag.glsl");
	if (planetShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}


	/////////////////////////////////////////////////////MISC\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	playerController.CreateGrid(&planeModel);
	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}

	
	////////////////////////////////////////////////////////////END\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	printf("On Create finished!!!!!");
	return true;


}

void Scene3g::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);

	


	friendlyShipModel.OnDestroy();

	enemyShipModel.OnDestroy();

	bulletModel.OnDestroy();

	sphereModel.OnDestroy();

	playerController.OnDestroy();

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

	bulletShader->OnDestroy();
	delete bulletShader;

	planetShader->OnDestroy();
	delete planetShader;


	planet.OnDestroy();


}

void Scene3g::HandleEvents(const SDL_Event& sdlEvent) {


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
		case SDL_SCANCODE_F:
			//TEMPORARY DELETE THIS LATER
			playerFleet[activeShip]->Fire();
			break;
		}


		break;


		//below is SUUUUUUPER temporary
		//will be in collision namespace once its up and running
		


			//centrePoint = VMath::normalize(centrePoint4); //if I normalize the centre point I get the wrong answer 

			



			

		
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

void Scene3g::Update(const float deltaTime) {

	enemySpawnPoint.Update(deltaTime);
	if (enemySpawnPoint.canSpawn == true) {
		enemyFleet.push_back(new EnemyShip(enemySpawnPoint.position, &enemyShipModel));
		enemyFleet.back()->OnCreate();
		enemySpawnPoint.canSpawn = false;
	}
	
	if (isGameRunning) {
		playerController.Update(deltaTime);
		

		if (playerController.has3DClick) {

			
			shipWaypoint = playerController.getClickPos();
			playerFleet[activeShip]->moveToDestination(shipWaypoint);
			
			
			
		}

		//super temporay will need to be moved
		if (playerController.hasDQLine) {


			DualQuat line = playerController.getLine();
			line = DQMath::normalize(line);

			


			//loop through the spheres
			for (int i = 0; i < playerFleet.size(); i++) {
				Vec4 centrePoint = playerFleet[i]->transform.getPos();
				
				Flector M = centrePoint * line;
				Plane pM = M.plane;//grab the plane part of the flector
				Vec4 vM = M.point;

				float radius = playerFleet[i]->collisionSphere->r;//hardcoded for now until sphere and collision classes are up
				float dSquared = (radius * radius) - (VMath::mag(vM) * VMath::mag(vM)); // d^2 = r^2 - (*<M>3)^2
				//pretty sure I'm doing this part wrong (*<M>3)^2

				if (dSquared > 0) { //if d^2 > 0 then the line and sphere intersect
					activeShip = i;

					Vec4 i1 = line ^ (M.plane + Plane(0, 0, 0, sqrt(dSquared))) * -1;
					i1 = i1 / i1.w;
					i1.print("Intersection Point 1");



					Vec4 i2 = line ^ (M.plane + Plane(0, 0, 0, -sqrt(dSquared))) * -1;
					i2 = i2 / i2.w;
					i2.print("Intersection Point 2");
				}
				


			}
		}

		//below can probably be a recursive function?
		for (FriendlyShip* ship : playerFleet) { //first we loop through every oen of the player's ships
			
			ship->color = BLUE;//all friendly ships are blue except the active ship TEMPORARY REMOVE LATER
			
			
			std::vector<Bullet*> temp = ship->getBullets(); //get a reference to all the bullets that ship has fired

			

			for (EnemyShip* targetShip : enemyFleet) { //now loop through each enemy to detect if an enmy is in range
				if (ship->canFire == true) { //before checking if enemy is in range check if the ship is even allowed to shoot yet
					if (COLLISION::SphereSphereCollisionDetected(&ship->detectionSphere, targetShip->collisionSphere)) {	//check if enemy ship is in range
					
						ship->targetDirection = VMath::normalize(targetShip->transform.getPos() - ship->transform.getPos()); //if the enemy is in range get the direction from our ship to the enemy
						ship->Fire();//fire a bullet at the enemy
						std::vector<Bullet*> temp = ship->getBullets(); //if the ship fired just update the temp list of bullets
					}
				}
				
				for (Bullet* bullet : temp) { //now we loop through each bullet to see if any would hit this enemy ship
					if (COLLISION::SphereSphereCollisionDetected(bullet->collisionSphere, targetShip->collisionSphere)) {
						bullet->deleteMe = true; //tell the ship to delete the bullet
						targetShip->Hit();
					}
				}
			}
			

			ship->Update(deltaTime);//call their update functions
			ship->displayRange = false;
		}
		
		playerFleet[activeShip]->color = GREEN;	//temporary to turn the selected ship green
		playerFleet[activeShip]->displayRange = true;

		for (int i = 0; i < enemyFleet.size(); i++) {
			enemyFleet[i]->Update(deltaTime);
			if (enemyFleet[i]->deleteMe) {
				enemyFleet[i]->OnDestroy();
				delete enemyFleet[i];
				enemyFleet[i] = nullptr;
				enemyFleet.erase(std::remove(enemyFleet.begin(), enemyFleet.end(), nullptr), enemyFleet.end());
			}
			else {
				enemyFleet[i]->Update(deltaTime);
			}

			
		}

	}

	
	
}

void Scene3g::Render() const {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	playerController.camera.RenderSkyBox();

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	

	


	for (FriendlyShip* ship : playerFleet) {
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		glUseProgram(shader->GetProgram());
		glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		ship->Render(shader);




		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //temporary line
		glUseProgram(bulletShader->GetProgram());
		glUniformMatrix4fv(bulletShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(bulletShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		ship->RenderBullets(bulletShader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //temporary line
	}



	for (EnemyShip* ship : enemyFleet) {
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		if (ship->deleteMe == false) { //shouldn't have to have this if here...
			glUseProgram(shader->GetProgram());
			glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			ship->Render(shader);
		}
	}

	
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	playerController.Render(shader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //temporary line
	glUseProgram(planetShader->GetProgram());
	glUniformMatrix4fv(planetShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(planetShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	glUniform4fv(planetShader->GetUniformID("lightPos"), 1, lightPos);
	glUniform4fv(planetShader->GetUniformID("cameraPos"), 1, playerController.camera.transform.getPos());
	planet.Render(planetShader);
	


	glUseProgram(0);
}




