#include <glew.h>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "SceneGameplay.h"
#include <MMath.h>
#include <QMath.h>
#include <VMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Model.h"
#include "AudioManager.h"

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

SceneGameplay::SceneGameplay(Window* window_) : shader{ nullptr }, 
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
	io.Fonts->AddFontFromFileTTF("./fonts/Ethnocentric Rg It.otf", 10.0f);
	// Adjust path and size as needed
	io.FontDefault = io.Fonts->Fonts.back(); // Set as default font (optional)
}

SceneGameplay::~SceneGameplay() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool SceneGameplay::OnCreate() {
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




	audioManager = new AudioManager();
	if (audioManager->OnCreate() == false) {
		std::cout << "Audio failed ... we have a problem\n";
	}

	SoundEngine->setSoundVolume(0.4f);
	SoundEngine->play2D("audio/BackGroundMusic2.mp3", true); // Audio For Game 

	createModels();
	createActors();
	createShaders();
	createClickGrid();


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

void SceneGameplay::OnDestroy() {
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

	audioManager->OnDestroy();
	delete audioManager;
}

void SceneGameplay::HandleEvents(const SDL_Event& sdlEvent) {


	playerController.handleEvents(sdlEvent);
	//basically whats happening here is that the player controller has a boolean flag thats basically saying "I have something to tell you scenemanager"
	//the scene knows to check for this flag and to recieve the message so the playercontroller does not need to have a reference to the scene
	//its basically just throwing out this variable and hoping something is listening
	//closest thing I could come up with to a delegate/event dispatcher
	ImGui_ImplSDL2_ProcessEvent(&sdlEvent); // ImGui HandleEvents

	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_P:
			//allows us to pause and unpause time, whoah.
			isGameRunning = !isGameRunning;
			Debug::Info("Paused", __FILE__, __LINE__);
			break;
		}
		break;
	}
}

void SceneGameplay::Update(const float deltaTime) {


	playerController.Update(deltaTime);

	//MUSIC VOL
	SoundEngine->setSoundVolume(volumeSlider);
	//SFX VOL
	audioManager->SoundEngine->setSoundVolume(sfxSlider);

	if (!isGameRunning) return;


	planet.Update(deltaTime);

	timeElapsed += deltaTime;

	SpawnEnemy(deltaTime);
	SetActiveShip();
	UpdatePlayerFleet(deltaTime);
	UpdateEnemyFleet(deltaTime);

	planet.Update(deltaTime);
	if (planet.GetHealth() < 0) {
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
		spawnTimer = 0.0f; // Reset the spawn timer
	}
}

void SceneGameplay::Render() {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	playerController.camera.RenderSkyBox();

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
		if (ship->deleteMe == false) { //don't render a ship thats to be deleted
			glUseProgram(friendlyShipShader->GetProgram());
			glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
			glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, PURPLE);
			glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
			glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, RED);
			ship->Render(friendlyShipShader);
			if (isGameRunning) ship->exhaustTrail.Render(particleShader, computeShader); //if the games runnign render the associated particle system too

		}
	}




	glUseProgram(planetShader->GetProgram());
	glUniformMatrix4fv(planetShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(planetShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	glUniform3fv(planetShader->GetUniformID("lightPos"), 1, lightPos);
	glUniform3fv(planetShader->GetUniformID("cameraPos"), 1, playerController.transform.getPos());
	planet.Render(planetShader);


	for (Explosion* explosion : explosions) {
		explosion->RenderDebris(bulletShader);
		explosion->Render(particleShader, computeExplosion);
	}

	for (FriendlyShip* ship : playerFleet) {

		glUseProgram(friendlyShipShader->GetProgram());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
		glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, Vec4(shipColor.x, shipColor.y, shipColor.z, shipColor.w));
		glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
		glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, BLUE);
		ship->Render(friendlyShipShader);

		glUseProgram(bulletShader->GetProgram());
		glUniformMatrix4fv(bulletShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(bulletShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(bulletShader->GetUniformID("cameraPos"), 1, playerController.transform.getPos());
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
		glUniform3fv(selectionShader->GetUniformID("cameraPos"), 1, playerController.transform.getPos());
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
			cursorSphere.Render(shader);
		}



		glUseProgram(gridShader->GetProgram());
		glUniformMatrix4fv(gridShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(gridShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		playerController.Render(gridShader);
	}
		
		glUseProgram(0);
}

void SceneGameplay::RenderIMGUI()
{
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



	//Pause Menu Creation (A lot of sloppy alignment but looks okay now)
	if (!isGameRunning)
	{
		//Begin Pause Menu
		ImGui::Begin("Pause Menu", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		float width = 0.0f;
		ImGui::SetWindowFontScale(2.0);
		width = ImGui::CalcTextSize("Game Paused").x;
		AlignForWidth(width);
		ImGui::Text("Game Paused", ImGuiWindowFlags_AlwaysAutoResize);

		//Okay so sliders are working.
		const ImGuiSliderFlags flags_for_sliders = ImGuiSliderFlags_None;
		ImGui::SetWindowFontScale(1.2);
		width = ImGui::CalcTextSize("Music Volume").x;
		AlignForWidth(width);
		ImGui::Text("Music Volume");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.95f);
		ImGui::SliderFloat("##1", &volumeSlider, 0.0f, 1.0f, "%.3f", flags_for_sliders);
		width = ImGui::CalcTextSize("Sfx Volume").x;
		AlignForWidth(width);
		ImGui::Text("Sfx Volume");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.95f);
		ImGui::SliderFloat("##2", &sfxSlider, 0.0f, 1.0f, "%.3f", flags_for_sliders);

		//Primary Color Slider works
		width = ImGui::CalcTextSize("Ship Color").x;
		AlignForWidth(width);
		ImGui::Text("Ship Color");
		//Probably store this in scene so we can pass it to the shader (NOW IN 3g.h file)
		ImGui::ColorButton("MyColor##3c", *(ImVec4*)&shipColor, ImGuiColorEditFlags_NoBorder, ImVec2(ImGui::GetWindowWidth() * 0.95f, 20));
		ImGui::ColorPicker3("##MyColor##5", (float*)&shipColor, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

		//Three Buttons to Unpause (Even though you can press P), Restart Scene, or Quit to title which was moved to this window.
		width = 150.0f;
		AlignForWidth(width);
		if (ImGui::Button("Unpause", ImVec2(150, 30))) isGameRunning = true;
		AlignForWidth(width);
		if (ImGui::Button("Restart", ImVec2(150, 30)))
		{
			SoundEngine->drop();
			restartBool = true;
		}
		AlignForWidth(width);
		if (ImGui::Button("Quit to Title", ImVec2(150, 30)))
		{
			SoundEngine->drop();
			switchButton = true;
		}

		//End Pause Menu
		ImGui::End();
	}
	ImGui::PopFont(); // Pop the font after usage 
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SceneGameplay::SpawnEnemy(const float deltaTime)
{
	for (int i = 0; i < enemySpawnerCount; i++) {

		enemyFleetSpawners[i].Update(deltaTime);
		if (enemyFleetSpawners[i].canSpawn == true) {
			enemyIndex++;
			enemyFleet.push_back(new EnemyShip(enemyFleetSpawners[i].position, &enemyShipModel, ENEMY_DEFAULT_HEALTH));
			enemyFleet.back()->SetAudioManager(audioManager);
			enemyFleet.back()->OnCreate();
			enemyFleet.back()->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh);
			enemyFleet.back()->setIndex(enemyIndex);
			enemyFleetSpawners[i].canSpawn = false;
		}
	}


}

void SceneGameplay::SetActiveShip()
{

	playerController.calculateLine(); //get the raycast into the screen

	int i = 0;
	for (FriendlyShip* ship : playerFleet) {
		if (COLLISION::LineSphereCollisionDetected(ship->collisionSphere, playerController.getLine())) {

			selectionSphere.meshColour = GREEN;
			selectionSphere.transform.setParent(ship->transform.toModelMatrix());
			isMouseOverShip = true;
			if (playerController.has3DClick) {
				activeShip = i;
				isGivingOrders = true;

				playerController.has3DClick = false;
				//SoundEngine->play2D("audio/SelectionSound.mp3", false); // Audio For Selection Ship
				audioManager->PlaySound2D("Ship_Selected");
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



	cursorSphere.transform.setPos(playerController.hoverPos);

	if (playerController.has3DClick && isGivingOrders) {

		if (activeShip >= 0) {
			shipWaypoint = playerController.getClickPos();

			playerFleet[activeShip]->MoveToDestination(shipWaypoint);
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

void SceneGameplay::UpdatePlayerFleet(const float deltaTime)
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

void SceneGameplay::RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime)
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

void SceneGameplay::UpdateEnemyFleet(const float deltaTime)
{
	for (int i = 0; i < enemyFleet.size(); i++) {
		enemyFleet[i]->Update(deltaTime);

		if (enemyFleet[i]->deleteMe) {
			DestroyEnenmy(i);
		}
		else {
			enemyFleet[i]->Update(deltaTime);

			if (COLLISION::SphereSphereCollisionDetected(enemyFleet[i]->collisionSphere, planet.collisionSphere)) {
				planet.takeDamage(enemyFleet[i]->damage);
				audioManager->PlaySound2D("Planet_Hit");
				DestroyEnenmy(i);
			}


		}
	}
}



void SceneGameplay::DestroyEnenmy(int index)
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
		if (explosions[explosions.size() - 1]->OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh, &debris) == false) {
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

void SceneGameplay::GameOver()
{


	SaveStats();
	audioManager->PlaySound2D("Game_Over");
	gameOverBool = true;

}


void SceneGameplay::SaveStats() {
	// Open a file in append mode
	std::ofstream outFile("Leaderboard.txt", std::ios::app);

	// Check if the file is open
	if (outFile.is_open()) {
		// Write the data to the file
		outFile << timeElapsed << " " << score << " " << "\n";
		outFile.close();

		std::cout << "Data appended to file successfully." << std::endl;
	}
	else {
		std::cerr << "Unable to open file for writing." << std::endl;
	}
}




void SceneGameplay::createModels()
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

void SceneGameplay::createActors()
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
		ship->SetAudioManager(audioManager);
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
		playerFleet[i]->SetAudioManager(audioManager);
		playerFleet[i]->closestEnemy = enemyFleet.back();	// Set initail target
		playerFleet[i]->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh);
	}

	planet = Planet(PLANET_RADIUS, PLANET_HEALTH, &sphereModel, ORIGIN);
	planet.OnCreate();

	selectionSphere = Actor(Transform(), &sphereModel);
	enemySelectionSphere = Actor(Transform(), &sphereModel);
	cursorSphere = Actor(Transform(), &sphereModel);
	cursorSphere.meshColour = ORANGE;
	selectionSphere.transform.setPos(0, 0, -0.5);
}

void SceneGameplay::createShaders()
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

void SceneGameplay::createClickGrid()
{
	playerController.CreateGrid(&planeModel);
	playerController.setPlayerBounds(planet.collisionSphere->r + 10, 150);
	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}
}