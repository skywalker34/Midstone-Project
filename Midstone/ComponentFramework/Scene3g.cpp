#include <glew.h>
#include <iostream>
#include <fstream>
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

#include "ComputeShader.h"

#include <chrono>


void AlignForWidth(float width, float alignment = 0.5f)
{
	ImGuiStyle& style = ImGui::GetStyle();
	float avail = ImGui::GetContentRegionAvail().x;
	float off = (avail - width) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

Scene3g::Scene3g(Window* window_) : shader{ nullptr }, 
drawInWireMode{ false } {
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

	// Load the font 
	io.Fonts->AddFontFromFileTTF("./fonts/Comic Sans MS.ttf", 16.0f);
	// Adjust path and size as needed
	io.FontDefault = io.Fonts->Fonts.back(); // Set as default font (optional)
}

Scene3g::~Scene3g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene3g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);


	


	computeShader = new ComputeShader("shaders/computer.glsl");	//create the compute shader
	if (computeShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	loadVertsToBuffer = new Shader("shaders/loadVerticiesVert.glsl", "shaders/loadVerticiesFrag.glsl");
	if (loadVertsToBuffer->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	particleShader = new Shader("shaders/particleVert.glsl", "shaders/particleFrag.glsl");
	if (particleShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	particleMesh = new Mesh("meshes/Mario.obj");
	particleMesh->OnCreate();


	//SoundEngine->setSoundVolume(0.4f);
	//SoundEngine->play2D("audio/BackGroundMusic2.mp3", true); // Audio For Game 
	



	createModels();
	createActors();
	createShaders();
	createClickGrid();

	testMesh = new Mesh("meshes/Sphere.obj");
	testMesh->OnCreate();

	debris = Model("Debris.obj");
	debris.OnCreate();
	
	enemyFleetSpawners.push_back(EnemySpawner(100.0f, 15, 5));
	printf("On Create finished!!!!!");
	


	computeExplosion = new ComputeShader("shaders/Explosion.glsl");	//create the compute shader
	if (computeExplosion->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}
	computeReset = new ComputeShader("shaders/ResetParticles.glsl");	//create the compute shader
	if (computeReset->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	for (int i = 0; i < startingExplosions; i++) {
		explosions.push_back(new Explosion());
		if (explosions[i]->OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh, &debris) == false) {
			std::cout << "Explosion failed ... we have a problem\n";
		}
	}
	
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

	for (Explosion* explosion : explosions) {
		explosion->OnDestroy();
		delete explosion;
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


	computeShader->OnDestroy();
	delete computeShader;

	loadVertsToBuffer->OnDestroy();
	delete loadVertsToBuffer;

	lineShader->OnDestroy();
	delete lineShader;


	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();


	SoundEngine->drop(); // Removes Sound from scene
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
			Debug::Info("Paused", __FILE__, __LINE__);
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

	
	playerController.Update(deltaTime);

	SoundEngine->setSoundVolume(volumeSlider);

	if (!isGameRunning) return;


	planet.Update(deltaTime);
	

	timeElapsed += deltaTime;

	SpawnEnemy(deltaTime);
	SetActiveShip();
	UpdatePlayerFleet(deltaTime);
	UpdateEnemyFleet(deltaTime);

	planet.Update(deltaTime);
	if (planet.GetHealth() < 0) {
		//temp for now
		GameOver();
	}

	for (Explosion* explosion : explosions) {
		explosion->Update(deltaTime);
	}


	static float spawnTimer = 0.0f; // Timer for spawning
	spawnTimer += deltaTime;

	if (spawnTimer >= 60.0f) { //spawn an enemySpawner every minute
		enemyFleetSpawners.push_back(EnemySpawner(200.0f, 5, 5));
		enemySpawnerCount++;
		//std::cout << "Enemy Spawners: " << enemySpawnerCount << std::endl;
		spawnTimer = 0.0f; // Reset the spawn timer
	}

	//testModelMat = MMath::translate(Vec3(0, 0, 70)) * MMath::scale(5, 5, 5);

	//std::cout << std::endl << "Score: " << score << std::endl;
	//std::cout << "Time Elapsed " << timeElapsed << std::endl;
}

void Scene3g::Render() {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	playerController.camera.RenderSkyBox();

	/*if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}*/



	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(lineShader->GetProgram());
	glUniformMatrix4fv(lineShader->GetUniformID("projection"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(lineShader->GetUniformID("view"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	glUniformMatrix4fv(lineShader->GetUniformID("model"), 1, GL_FALSE, testLine.transform.toModelMatrix());
	testLine.draw();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	if (activeShip >= 0) {

		glUseProgram(lineShader->GetProgram());
		glUniformMatrix4fv(lineShader->GetUniformID("projection"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(lineShader->GetUniformID("view"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniformMatrix4fv(lineShader->GetUniformID("model"), 1, GL_FALSE, pathLine.transform.toModelMatrix());
		pathLine.draw();
	}

	







	for (EnemyShip* ship : enemyFleet) {
		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		if (ship->deleteMe == false) { //shouldn't have to have this if here...
		
			//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
			glUseProgram(friendlyShipShader->GetProgram());
			glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
			glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, PURPLE);
			glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
			glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, RED);
			ship->Render(friendlyShipShader);





			if (isGameRunning) ship->exhaustTrail.Render(particleShader, computeShader);

		}
	}




	glUseProgram(planetShader->GetProgram());
	glUniformMatrix4fv(planetShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(planetShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	glUniform3fv(planetShader->GetUniformID("lightPos"), 1, lightPos);
	glUniform3fv(planetShader->GetUniformID("cameraPos"), 1, playerController.camera.transform.getPos());
	planet.Render(planetShader);


	for (Explosion* explosion : explosions) {
		explosion->RenderDebris(bulletShader);
		explosion->Render(particleShader, computeExplosion);
	}

	for (FriendlyShip* ship : playerFleet) {

		//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, ship->shipModelMatrix);
		glUseProgram(friendlyShipShader->GetProgram());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
		glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, ORANGE);
		glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
		glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, BLUE);
		ship->Render(friendlyShipShader);





		glUseProgram(bulletShader->GetProgram());
		glUniformMatrix4fv(bulletShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(bulletShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(bulletShader->GetUniformID("cameraPos"), 1, playerController.camera.transform.getPos());
		ship->RenderBullets(bulletShader);




		if (ship->isMoving && isGameRunning) {
			ship->exhaustTrail.Render(particleShader, computeShader);
		}



		if (isGivingOrders) 
		{


			glUseProgram(shader->GetProgram());
			glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			ship->RenderRange(shader);
		}


		
	}

	if (isMouseOverShip)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUseProgram(selectionShader->GetProgram());
		glUniformMatrix4fv(selectionShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(selectionShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(selectionShader->GetUniformID("cameraPos"), 1, playerController.camera.transform.getPos());
		selectionSphere.Render(selectionShader);
		glDisable(GL_BLEND);
	}

	if (isGivingOrders) 
	{

		if (!isMouseOverShip) 
		{
			glUseProgram(shader->GetProgram());
			glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, testModelMat);
			glUniform4fv(shader->GetUniformID("meshColor"), 1, ORANGE);
			testMesh->Render(GL_TRIANGLES);
		}



		glUseProgram(gridShader->GetProgram());
		glUniformMatrix4fv(gridShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(gridShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		playerController.Render(gridShader);
	}





		// IMGUI STUFF
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();

		//This is the font stuff if you can find a working one then yeah. But otherwise im gonna keep it default for now.
		//ImGuiIO& io = ImGui::GetIO();
		//ImFontConfig config;
		//config.OversampleH = 2;
		//io.Fonts->AddFontDefault();
		//ImFont* textFont = io.Fonts->AddFontFromFileTTF("./fonts/Comic Sans MS.ttf", 23.0f, &config);
		//IM_ASSERT(textFont != NULL);
		//io.Fonts->Build();



		// IMGUI STUFF 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		bool p_open = false;
		// Apply the font 
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		// Use the loaded font
		ImGui::Begin("Timer and Score", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("Time = %f", timeElapsed);
		ImGui::Text("Score = %i", score);
		ImGui::Text("Planet Health: = %i", planet.GetHealth());
		ImGui::End();

		//Don't need because we its in pause menu now.
		//ImGui::Begin("QuitButton", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		//if (ImGui::Button("Quit to Title", ImVec2(150, 30))) switchButton = true;
		//ImGui::End();

		//Pause Menu Creation
		if (!isGameRunning)
		{
			//Begin Pause Menu
			ImGui::Begin("Pause Menu", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			float width = 0.0f;
			width = ImGui::CalcTextSize("Paused").x;
			AlignForWidth(width);
			ImGui::Text("Paused");

			//Test for Volume Sliders (GET ANDY TO DO SOMETHING WITH THIS BECAUSE I DON'T KNOW ABOUT HOW THE SOUND ENGINE IS CONFIGURED.
			//YOU PROBABLY WANT MORE THAN JUST SOUNDENGINE AND SOUNDENGINEFLYING)
			const ImGuiSliderFlags flags_for_sliders = ImGuiSliderFlags_None;
			ImGui::Text("Music Volume");
			ImGui::SliderFloat("##1", &volumeSlider, 0.0f, 1.0f, "%.3f", flags_for_sliders);
			ImGui::Text("Sfx Volume (NOT WORKING YET)");
			ImGui::SliderFloat("##2", &sfxSlider, 0.0f, 1.0f, "%.3f", flags_for_sliders);

			//Test For Ship Color (Or whatever this is used for but not working at the moment bear with me) Need to ask Andrew L about these
			ImGui::Text("Ship Color");
			//Probably store this in scene so we can pass it to the shader
			static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
			ImGui::ColorButton("MyColor##3c", *(ImVec4*)&color, ImGuiColorEditFlags_NoBorder), ImVec2(80, 80);
			ImGui::ColorPicker3("##MyColor##5", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

			//Three Buttons to Unpause (Even though you can press P), Restart Scene, or Quit to title which was moved to this window.
			if (ImGui::Button("Unpause", ImVec2(150, 30))) isGameRunning = true;
			if (ImGui::Button("Restart", ImVec2(150, 30))) restartBool = true;
			if (ImGui::Button("Quit to Title", ImVec2(150, 30))) switchButton = true;

			//End Pause Menu
			ImGui::End();
		}
		ImGui::PopFont(); // Pop the font after usage 
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glUseProgram(0);
}

void Scene3g::SpawnEnemy(const float deltaTime)
{
	for (int i = 0; i < enemySpawnerCount; i++) {

		enemyFleetSpawners[i].Update(deltaTime);
		if (enemyFleetSpawners[i].canSpawn == true) {
			enemyIndex++;
			enemyFleet.push_back(new EnemyShip(enemyFleetSpawners[i].position, &enemyShipModel));
			enemyFleet.back()->OnCreate();
			enemyFleet.back()->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh);
			enemyFleet.back()->setIndex(enemyIndex);
			enemyFleetSpawners[i].canSpawn = false;
		}
	}

	
}

void Scene3g::SetActiveShip()
{

	playerController.calculateLine(); //get the raycast into the screen

	int i = 0;
	for (FriendlyShip* ship : playerFleet) {
		if (COLLISION::LineSphereCollisionDetected(ship->collisionSphere, playerController.getLine())) {
			//selectionSphere.transform = ship->transform;

			selectionSphere.meshColour = GREEN;
			selectionSphere.transform.setParent(ship->transform.toModelMatrix());
			isMouseOverShip = true;
			if (playerController.has3DClick) {
				activeShip = i;
				isGivingOrders = true;
				
				playerController.has3DClick = false;
        SoundEngine->play2D("audio/SelectionSound.mp3", false); // Audio For Selection Ship
			}
			break;
		}
		else {
			isMouseOverShip = false;
		}
		i++;

	}

	
	if (isGivingOrders) { //if player has a ship selected and clicks on an enemy we want to attack that enenmy
		for (EnemyShip* ship : enemyFleet) {
			if (COLLISION::LineSphereCollisionDetected(ship->collisionSphere, playerController.getLine())) { //check for collision between player raycast and the enemy ships



				selectionSphere.transform.setParent(ship->transform.toModelMatrix()); //reparent the model matrix to the ship
				isMouseOverShip = true;
				selectionSphere.meshColour = RED;

				if (playerController.has3DClick) {
					playerController.has3DClick = false;
					shipWaypoint = ship->transform.getPos();
					isGivingOrders = false;

					printf("ATTACKING! \n");

					playerFleet[activeShip]->isChasing = true;
					playerFleet[activeShip]->activeTarget = ship;
					activeShip = -1;
				}
				break;
			}
			else {
				isMouseOverShip = false;
			}
		}
	}

	testModelMat = MMath::translate(playerController.hoverPos) * MMath::scale(1, 1, 1);
	if (playerController.has3DClick && isGivingOrders) {

		if (activeShip >= 0) {
			shipWaypoint = playerController.getClickPos();
			
			playerFleet[activeShip]->moveToDestination(shipWaypoint);
			isGivingOrders = false;
			activeShip = -1;
		}
	}



	if (playerController.hasCanceledOrder) {
		isGivingOrders = false;
		activeShip = -1;

		playerController.hasCanceledOrder = false;

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

	if (activeShip >= 0) {
		isGivingOrders = true;
		playerFleet[activeShip]->displayRange = true;
		if (playerFleet[activeShip]->isMoving) {
			pathLine.RecalculateLine(playerFleet[activeShip]->transform.getPos(), playerFleet[activeShip]->destination);
		}
	}

	

}

void Scene3g::RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime)
{
	if (!ship->isMoving) {
		if (rotationTimer <= 1) {
			ship->FindClosestEnemy(targetShip);
			rotationTimer += deltaTime;
		}
		else {
			rotationTimer = 0;
		}
		
		if (ship->currentTargetIndex == targetShip->shipIndex) {
			Vec3 targetDirection = targetShip->transform.getPos() - ship->transform.getPos();
			Quaternion targetQuad = QMath::lookAt(targetDirection, UP);
			ship->transform.setOrientation(targetQuad);
			ship->initialDirection = targetDirection;
			testLine.RecalculateLine(targetShip->aimingPoint, ship->transform.getPos());
		}
		
		if (ship->isSwitchingTarget) {
			ship->slerpT = ship->slerpT + deltaTime;
			if (ship->slerpT >= 1) {
				ship->closestEnemy = ship->potentialTarget;
				ship->currentTargetIndex = ship->closestEnemy->shipIndex;
				ship->isSwitchingTarget = false;
				ship->slerpT = 0;
			}
			testLine.RecalculateLine(ship->potentialTarget->aimingPoint, ship->transform.getPos());
		}
	}
}

void Scene3g::UpdateEnemyFleet(const float deltaTime)
{
	for (int i = 0; i < enemyFleet.size(); i++) {
		enemyFleet[i]->Update(deltaTime);
		
		if (enemyFleet[i]->deleteMe) {
			DestroyEnenmy(i);
		}
		else {
			enemyFleet[i]->Update(deltaTime);

			if (COLLISION::SphereSphereCollisionDetected(enemyFleet[i]->collisionSphere, planet.collisionSphere)) {
				planet.takeDamage(1);

				
				SoundEngine->setSoundVolume(0.2f);
				SoundEngine->play2D("audio/PlanetGotHit.mp3", false); // Planet Got Hit Sound

				
				DestroyEnenmy(i);
			}


		}
	}
}



void Scene3g::DestroyEnenmy(int index)
{
	score++;


	Vec3 explosionPos = enemyFleet[index]->transform.getPos();
	bool explosionAvailable = false;
	for (Explosion* explosion : explosions) {
		if (!explosion->animComplete) {
			explosionAvailable = true; 
			explosion->setPos(explosionPos);
			explosion->ResetExplosion(computeReset);
		}
	}

	if (!explosionAvailable) {
		//if there are no explosions availabel then cretae a new one
		explosions.push_back(new Explosion());
		if (explosions[explosions.size()-1]->OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh, &debris) == false) {
			std::cout << "Explosion failed ... we have a problem\n";
		}
		explosions[explosions.size() - 1]->setPos(explosionPos);
		explosions[explosions.size() - 1]->ResetExplosion(computeReset);
	}

	enemyFleet[index]->OnDestroy();
	delete enemyFleet[index];
	enemyFleet[index] = nullptr;
	enemyFleet.erase(std::remove(enemyFleet.begin(), enemyFleet.end(), nullptr), enemyFleet.end());
}

void Scene3g::GameOver()
{
	//END GAME LOGIC HERE PLEASE
	std::cout << "\033[32m" << "GAMEOVER!" << "\033[0m" << std::endl;
	SaveStats();
	SoundEngine->play2D("audio/GameoverSound.mp3", false);
	gameOverBool = true;
	
}


void Scene3g::SaveStats() {
	// Open a file in append mode
	std::ofstream outFile("Leaderboard.txt", std::ios::app);

	// Check if the file is open
	if (outFile.is_open()) {
		// Write the data to the file
		outFile << timeElapsed << " " << score << " " << "\n";

		//Old format for how Leaderboard.txt was layed out
		//outFile << "Score: " << score << "\n";
		//outFile << "Time: " << timeElapsed << "\n";
		//outFile << "-" << "\n";

		// Close the file
		outFile.close();

		std::cout << "Data appended to file successfully." << std::endl;
	}
	else {
		std::cerr << "Unable to open file for writing." << std::endl;
	}
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


	planeModel = Model("Plane.obj", std::vector<std::string>{"Grid3.png"});
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
		ship->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh);
		ship->setIndex(enemyIndex);
		enemyIndex++;
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
		playerFleet[i]->closestEnemy = enemyFleet.back();	// Set initail target
		playerFleet[i]->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh);
	}

	planet = Planet(30.0f, 50, &sphereModel, ORIGIN);
	planet.OnCreate();

	selectionSphere = Actor(Transform(), &sphereModel);
	enemySelectionSphere = Actor(Transform(), &sphereModel);
	selectionSphere.transform.setPos(0, 0, -0.5);
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

	gridShader = new Shader("shaders/clickGridVert.glsl", "shaders/clickGridFrag.glsl");
	if (gridShader->OnCreate() == false) 
	{
		std::cout << "Shader failed ... we have a problem\n";
	}


	selectionShader = new Shader("shaders/selectionVert.glsl", "shaders/selectionFrag.glsl");
	if (selectionShader->OnCreate() == false)
	{
		std::cout << "Shader failed ... we have a problem\n";
	}
	lineShader = new Shader("shaders/lineVert.glsl", "shaders/lineFrag.glsl");
	if (lineShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

}

void Scene3g::createClickGrid()
{
	playerController.CreateGrid(&planeModel);
	playerController.setPlayerBounds(planet.collisionSphere->r + 10, 150);
	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}
}