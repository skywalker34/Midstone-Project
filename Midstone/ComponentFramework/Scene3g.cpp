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

Scene3g::Scene3g(Window* window_) : shader{ nullptr }, 
drawInWireMode{ true } {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
	window = window_;

	// ImGUI stuff for initialize from Scotties Vid.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//Setup Dear ImGui Style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window->getWindow(), window->getContext());
	ImGui_ImplOpenGL3_Init("#version 450");
}

Scene3g::~Scene3g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene3g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);

	createModels();
	createActors();
	createShaders();
	createClickGrid();

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

	friendlyShipShader->OnDestroy();
	delete friendlyShipShader;


	planet.OnDestroy();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Scene3g::HandleEvents(const SDL_Event& sdlEvent) {


	playerController.handleEvents(sdlEvent);
	//basically whats happening here is that the player controller has a boolean flag thats basically saying "I have something to tell you scenemanager"
	//the scene knows to check for this flag and to recieve the message so the playercontroller does not need to have a reference to the scene
	//its basically just throwing out this variable and hoping something is listening
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent); // ImGui HandleEvents

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

	if (!isGameRunning) return;
	
	timeElapsed += deltaTime;

	planet.Update(deltaTime);
	playerController.Update(deltaTime);

	SpawnEnemy(deltaTime);
	SetActiveShip();
	UpdatePlayerFleet(deltaTime);
	UpdateEnemyFleet(deltaTime);

	

	std::cout << std::endl << "Score: " << score << std::endl;
	std::cout << "Time Elapsed " << timeElapsed << std::endl;
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //temporary line
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		glUseProgram(friendlyShipShader->GetProgram());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
		glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, ORANGE);
		glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
		glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, BLUE);
		ship->Render(friendlyShipShader);




		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //temporary line
		glUseProgram(bulletShader->GetProgram());
		glUniformMatrix4fv(bulletShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(bulletShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(bulletShader->GetUniformID("cameraPos"), 1, playerController.camera.transform.getPos());
		ship->RenderBullets(bulletShader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //temporary line
	}



	for (EnemyShip* ship : enemyFleet) {
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		if (ship->deleteMe == false) { //shouldn't have to have this if here...
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //temporary line
			//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
			glUseProgram(friendlyShipShader->GetProgram());
			glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
			glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, PURPLE);
			glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
			glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, RED);
			ship->Render(friendlyShipShader);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //temporary line
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
	glUniform3fv(planetShader->GetUniformID("lightPos"), 1, lightPos);
	glUniform3fv(planetShader->GetUniformID("cameraPos"), 1, playerController.camera.transform.getPos());
	planet.Render(planetShader);

	// IMGUI STUFF
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	bool p_open = false;
	ImGui::Begin("Timer and Score", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::Text("Time = %f", timeElapsed);
	ImGui::Text("Score = %i", score);
	ImGui::End();

	ImGui::Begin("QuitButton", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::Button("Quit to Title", ImVec2(150,30));
	ImGui::End();

	ImGui::ShowDemoWindow();
	ImGui::Render(); // Calling This before CurrentScene render wont work
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glUseProgram(0);
}

void Scene3g::SpawnEnemy(const float deltaTime)
{
	enemySpawnPoint.Update(deltaTime);
	if (enemySpawnPoint.canSpawn == true) {
		enemyIndex++;
		enemyFleet.push_back(new EnemyShip(enemySpawnPoint.position, &enemyShipModel));
		enemyFleet.back()->OnCreate();
		enemyFleet.back()->setIndex(enemyIndex);
		enemySpawnPoint.canSpawn = false;
	}
}

void Scene3g::SetActiveShip()
{
	if (playerController.has3DClick) {
		shipWaypoint = playerController.getClickPos();
		playerFleet[activeShip]->moveToDestination(shipWaypoint);
	}

	if (playerController.hasDQLine) {
		DualQuat line = playerController.getLine();

		//loop through the spheres
		for (int i = 0; i < playerFleet.size(); i++) {
			if (COLLISION::LineSphereCollisionDetected(playerFleet[i]->collisionSphere, line))
			{
				activeShip = i;
			}
		}
	}
}

void Scene3g::UpdatePlayerFleet(const float deltaTime)
{
	//below can probably be a recursive function?
	for (FriendlyShip* ship : playerFleet) {																				//first we loop through every oen of the player's ships

		std::vector<Bullet*> temp = ship->getBullets();																		//get a reference to all the bullets that ship has fired

		for (EnemyShip* targetShip : enemyFleet) {																			//now loop through each enemy to detect if an enmy is in range
			RotateTowardEnemy(ship, targetShip, deltaTime);

			if (ship->canFire == true) {																					//before checking if enemy is in range check if the ship is even allowed to shoot yet
				if (COLLISION::SphereSphereCollisionDetected(&ship->detectionSphere, targetShip->collisionSphere)) {		//check if enemy ship is in range
					ship->targetDirection = VMath::normalize(targetShip->transform.getPos() - ship->transform.getPos());	//if the enemy is in range get the direction from our ship to the enemy
					ship->Fire();
					std::vector<Bullet*> temp = ship->getBullets();															//if the ship fired just update the temp list of bullets
				}
			}

			for (Bullet* bullet : temp) {																					//now we loop through each bullet to see if any would hit this enemy ship
				if (COLLISION::SphereSphereCollisionDetected(bullet->collisionSphere, targetShip->collisionSphere)) {
					bullet->deleteMe = true;																				//tell the ship to delete the bullet
					targetShip->Hit();
				}
			}
		}

		ship->Update(deltaTime);																							//call their update functions
		ship->displayRange = false;
	}

	playerFleet[activeShip]->displayRange = true;
}

void Scene3g::RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime)
{
	if (!ship->isMoving) {
		ship->FindClosestEnemy(targetShip);
		if (ship->currentTargetIndex == targetShip->shipIndex) {
			Quaternion targetQuad = QMath::lookAt(ship->closestEnemyPosition, UP);
			ship->transform.setOrientation(targetQuad);
		}
		else {
			ship->slerpT = ship->slerpT >= 1 ? 0 : ship->slerpT + deltaTime;
			ship->rotateTowardTarget(ship->closestEnemyPosition);
			ship->canFire = false;
			if (ship->slerpT >= 1) {
				ship->currentTargetIndex = targetShip->shipIndex;
			}
		}

	}
}

void Scene3g::UpdateEnemyFleet(const float deltaTime)
{
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

	/*for (EnemyShip* enemy : enemyFleet) {
		enemy->Update(deltaTime);
		if (enemy->deleteMe) {
			enemy->OnDestroy();
			delete enemy;
			enemy = nullptr;
			enemyFleet.erase(std::remove(enemyFleet.begin(), enemyFleet.end(), nullptr), enemyFleet.end());
		}
		else {
			enemy->Update(deltaTime);
		}
	}*/
}

void Scene3g::createModels()
{
	friendlyShipModel = Model("midstone_ship.obj", std::vector<std::string>{"Ship_Colour_Mask.png"});
	if (friendlyShipModel.OnCreate() == false) {
		printf("Model failed to load");
	}


	enemyShipModel = Model("Midstone_Enemy.obj", std::vector<std::string>{"EShip_Colour_Mask.png"});
	if (enemyShipModel.OnCreate() == false) {
		printf("Model failed to load");
	}

	bulletModel = Model("Bullet.obj");
	if (bulletModel.OnCreate() == false) {
		printf("Model failed to load");
	}

	sphereModel = Model("Sphere.obj", std::vector<std::string>{"planet_d.png", "planet_s.png", "cloud_map.png"});
	if (sphereModel.OnCreate() == false) {
		printf("Model failed to load");
	}


	planeModel = Model("Plane.obj");
	if (planeModel.OnCreate() == false) {
		printf("Model failed to load");
	}
}

void Scene3g::createActors()
{
	for (int i = 0; i < startingFleetSize; i++) {
		int numShips = startingFleetSize;
		float radius = 75.0f;
		float angle = 2.0f * M_PI * i / numShips;										// Calculate the angle for each ship
		float x = radius * cos(angle);													// Calculate x position
		float z = radius * sin(angle);													// Calculate z position

		enemyFleet.push_back(new EnemyShip(Vec3(x, 0, z), &enemyShipModel));

	}
	for (EnemyShip* ship : enemyFleet) {
		ship->OnCreate();
		ship->setIndex(enemyIndex);
	}

	for (int i = 0; i < startingFleetSize; i++) {
		playerFleet.push_back(new FriendlyShip(&friendlyShipModel, &bulletModel));
	}


	//spawn the ships in a radius around the planet

	int numShips = playerFleet.size();
	float radius = 50.0f;

	for (int i = 0; i < numShips; ++i)
	{
		float angle = 2.0f * M_PI * i / numShips;										// Calculate the angle for each ship
		float x = radius * cos(angle);													// Calculate x position
		float z = radius * sin(angle);													// Calculate z position


		playerFleet[i]->transform.setPos(Vec3(x, 0.0f, z));
		playerFleet[i]->OnCreate();
		playerFleet[i]->closestEnemyPosition = enemyFleet.back()->transform.getPos();	// Set initail target
	}

	planet = Planet(30.0f, 5, &sphereModel, ORIGIN);
	planet.OnCreate();
}

void Scene3g::createShaders()
{
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

	friendlyShipShader = new Shader("shaders/friendlyShipVert.glsl", "shaders/friendlyShipFrag.glsl");
	if (friendlyShipShader->OnCreate() == false)
	{
		std::cout << "Shader failed ... we have a problem\n";
	}
}

void Scene3g::createClickGrid()
{
	playerController.CreateGrid(&planeModel);
	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}
}
