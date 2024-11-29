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


SceneGameplay::SceneGameplay(Window* window_) : shader{ nullptr }
{
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





	computeExhaust = new ComputeShader("shaders/computer.glsl");	//create the compute shader
	if (computeExhaust->OnCreate() == false) {
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

	particleMesh = new Mesh("meshes/Particle.obj");
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
	createPlayerController();


	debrisModel = Model("Debris.obj");
	debrisModel.OnCreate();

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
		if (explosions[i]->OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh, &debrisModel) == false) {
			std::cout << "Explosion failed ... we have a problem\n";
		}
	}

	options.readOptions("options.txt");
	volumeSlider = options.musicVol;
	sfxSlider = options.sfxVol;
	shipColor = ImVec4(options.shipColour.x, options.shipColour.y, options.shipColour.z, options.shipColour.w);

	return true;

}

void SceneGameplay::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);

	//DELETE SHADERS
	shader->OnDestroy();
	delete shader;

	bulletShader->OnDestroy();
	delete bulletShader;

	planetShader->OnDestroy();
	delete planetShader;

	friendlyShipShader->OnDestroy();
	delete friendlyShipShader;

	gridShader->OnDestroy();
	delete gridShader;

	selectionShader->OnDestroy();
	delete selectionShader;

	lineShader->OnDestroy();
	delete lineShader;

	//DELETE COMPUTE SHADERS
	computeExhaust->OnDestroy();
	delete computeExhaust;

	computeExplosion->OnDestroy();
	delete computeExplosion;

	computeReset->OnDestroy();
	delete computeReset;

	loadVertsToBuffer->OnDestroy();
	delete loadVertsToBuffer;

	//DELETE MESH/MODELS

	particleMesh->OnDestroy();
	delete particleMesh;

	friendlyShipModel.OnDestroy();

	enemyShipModel.OnDestroy();

	bulletModel.OnDestroy();

	sphereModel.OnDestroy();

	planeModel.OnDestroy();

	debrisModel.OnDestroy();

	//DELETE WORLD OBJECTS

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

	planet.OnDestroy();

	audioManager->OnDestroy();
	delete audioManager;
	

	
	//CLEANUP GUI

	options.SaveOptions("options.txt", volumeSlider, sfxSlider, Vec4(shipColor.x, shipColor.y, shipColor.z, shipColor.w));

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SoundEngine->drop();
	

	window = nullptr; //prevent dangling pointers

	printf("All assets deleted");
}

void SceneGameplay::HandleEvents(const SDL_Event& sdlEvent) {


	playerController.handleEvents(sdlEvent); //handle events on the player controller sid eof things





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

	//MUSIC VOL
	SoundEngine->setSoundVolume(volumeSlider);
	//SFX VOL
	audioManager->SoundEngine->setSoundVolume(sfxSlider);




	if (!isGameRunning) return; //if the game is not running (paused) just return so nothing happens
	//call the individual updat functions / methods

	playerController.Update(deltaTime);
	planet.Update(deltaTime);

	SpawnEnemy(deltaTime);
	UpdatePlayerFleet(deltaTime);
	UpdateEnemyFleet(deltaTime);


	//this may not be good practice but its the best I've got
	//the events the player controller is handeling here need the most updated positions of the game objects, so its executed after the ship updates/ 
	PlayerControllerHandleEvents(); //handle the changes on tehs cene side of things

	planet.Update(deltaTime);
	if (planet.GetHealth() < 0) {
		isGameRunning = false;
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

	timeElapsed += deltaTime;
}

void SceneGameplay::Render() {
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	playerController.camera.RenderSkyBox();

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
		glUseProgram(friendlyShipShader->GetProgram());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
		glUniformMatrix4fv(friendlyShipShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
		glUniform3fv(friendlyShipShader->GetUniformID("lightPos"), 1, lightPos);
		glUniform4fv(friendlyShipShader->GetUniformID("primaryColour"), 1, PURPLE);
		glUniform4fv(friendlyShipShader->GetUniformID("secondaryColour"), 1, GREY);
		glUniform4fv(friendlyShipShader->GetUniformID("tertiaryColour"), 1, RED);
		if (ship->deleteMe == false) { //don't render a ship thats to be deleted
			ship->Render(friendlyShipShader);
			if (isGameRunning) ship->exhaustTrail.Render(particleShader, computeExhaust); //if the games runnign render the associated particle system too

		}
	}



	//these uniforms don't change so set them once
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

		//we can't set the common uniforms once, because this loop goes rendership->renderbullets so any ship after the first would use the bullet shader (no good)

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
			ship->exhaustTrail.Render(particleShader, computeExhaust);
		}


		if (isGivingOrders)  //if the ship is being commanded, show the player its range
		{
			glUseProgram(shader->GetProgram());
			glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			ship->RenderRange(shader);
		}
	}

	if (isMouseOverShip) //if the mouse is hoveing a ship, render the selection sphere
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
		//if the player is giving orders and their mouse is not over a ship, render the 3d cursor
		if (!isMouseOverShip)
		{
			glUseProgram(shader->GetProgram());
			glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
			glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
			cursorSphere.Render(shader);
		}
		//if player is giving orders, they need to see the grid
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
	float width = 0.0f;
	// Apply the font 
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
	// Use the loaded font

	if (isGameRunning && planet.GetHealth() >= 0)
	{
		ImGui::Begin("Timer and Score", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("Time = %f", timeElapsed);
		ImGui::Text("Score = %i", score);
		ImGui::Text("Planet Health: = %i", planet.GetHealth());
		ImGui::End();
	}


	//Pause Menu Creation (A lot of sloppy alignment but looks okay now)
	if (!isGameRunning && planet.GetHealth() >= 0)
	{
		//Begin Pause Menu
		ImGui::Begin("Pause Menu", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);;
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

	if (!isGameRunning && planet.GetHealth() < 0)
	{
		//Begin Name Entry
		ImGui::Begin("Name Entry", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowFontScale(2.0);
		width = ImGui::CalcTextSize("Name Entry").x;
		AlignForWidth(width);
		ImGui::Text("Name Entry");

		char buffer[256];
		//Get size of std::string
		size_t length = nameEntry.size();
		//If our name is somehow bigger than our buffer.
		if (length >= sizeof(buffer))
		{
			length = sizeof(buffer) - 1; 
		}
		std::memcpy(buffer, nameEntry.c_str(), length);
		buffer[length] = '\0'; 
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.93f);
		if (ImGui::InputText("##EnterName", buffer, sizeof(buffer))) nameEntry = buffer;
		width = 150.0f;
		AlignForWidth(width);
		if (ImGui::Button("Submit", ImVec2(150, 30)))
		{
			//End Game on button press and go to next scene
			GameOver();
		}
		//End Name Entry
		ImGui::End();
	}

	ImGui::PopFont(); // Pop the font after usage 
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SceneGameplay::SpawnEnemy(const float deltaTime)
{
	for (int i = 0; i < enemySpawnerCount; i++) {
		enemyFleetSpawners[i].Update(deltaTime);//update all the fleet spawners to tick their internal clocks

		if (enemyFleetSpawners[i].canSpawn == true) { //if any of them have a flagspatcher saying to spawn an enemy
			//do it 
			enemyIndex++;//increment the enenmy index. note this is not the index in the enenyfleet vector, this is a unique index for teh scene to keep track of eacch enenmy 
			//think of it like a serial code for the enemy ship
			enemyFleet.push_back(new EnemyShip(enemyFleetSpawners[i].position, &enemyShipModel, ENEMY_DEFAULT_HEALTH)); //construct the new enemy
			enemyFleet.back()->SetAudioManager(audioManager); //pass the enenmy the audio manager
			enemyFleet.back()->OnCreate(); //create the enemy
			enemyFleet.back()->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh); //create the enenmy exhaust trail
			enemyFleet.back()->setIndex(enemyIndex);//tell the enenmy its "serial number"
			enemyFleetSpawners[i].canSpawn = false;//use up the flagspatcher
		}
	}
}

void SceneGameplay::PlayerControllerHandleEvents()
{
	//the advent of the "Flagspatcher"
	//basically whats happening here is that the player controller has a boolean flag thats basically saying "I have something to tell you scenemanager"
	//the scene knows to check for this flag and to recieve the message so the playercontroller does not need to have a reference to the scene
	//its basically just throwing out this variable and hoping something is listening
	//closest thing I could come up with to a delegate/event dispatcher

	playerController.calculateLine(); //get the raycast into the screen

	isMouseOverShip = false; //assume our mouse is not over a ship

	int i = 0; //holds the index of the ship 
	for (FriendlyShip* ship : playerFleet) { //loop through player ships
		if (COLLISION::LineSphereCollisionDetected(ship->collisionSphere, playerController.getLine())) {

			selectionSphere.meshColour = GREEN; //make the selection sphere (the circle around a ship when you hover your mouse, greem)
			selectionSphere.transform.setParent(ship->transform.toModelMatrix());//set the selections sphere's parent to be the ship so it hovers over it
			isMouseOverShip = true;//remember our mouse is over a ship

			if (playerController.has3DClick) {
				//if the player has clicked, then whichever ship our mouse is over has been clicked on 
				activeShip = i;
				isGivingOrders = true; //tell teh scene we're giving orders to the ships now 

				playerController.has3DClick = false;//tell the ship its flagspatcher has been "heard"
				audioManager->PlaySound2D("Ship_Selected"); //play the sound
			}
			break;
		}
		else {
			//if the ray doesn't collide with any ship, set bool to false
			isMouseOverShip = false;

		}
		i++;

	}

	//if the player is giving orders, it means the player has clicked on a friendly ship and is now giving it command
	if (isGivingOrders) PlayerOrders(); //get the orders from the player,

	if (playerController.hasCanceledOrder) {
		isGivingOrders = false;
		activeShip = -1;

		playerController.hasCanceledOrder = false;

	}

}


void SceneGameplay::PlayerOrders()
{
	//move the 3d cursor to wherever the palyer's mouse is hovering (in 3d space)
	cursorSphere.transform.setPos(playerController.mouseHoverPos3D);

	//if player has a ship selected and clicks on an enemy we want to attack that enenmy
	for (EnemyShip* ship : enemyFleet) {
		if (COLLISION::LineSphereCollisionDetected(ship->collisionSphere, playerController.getLine())) { //check for collision between player raycast and the enemy ships
			selectionSphere.transform.setParent(ship->transform.toModelMatrix()); //reparent the model matrix to the ship

			isMouseOverShip = true;//our mouse is hovering a ship
			selectionSphere.meshColour = RED; //its an enenmy ship so make it red

			if (playerController.has3DClick) { //if player has clicked on an enemy ship
				playerController.has3DClick = false;//use up the flagspatcher
				//set the ship's waypoint to be the enemy's position
				shipWaypoint = ship->transform.getPos();
				isGivingOrders = false;//the order has been given so the player is no longer giving orders
				playerFleet[activeShip]->slerpT = 0;
				playerFleet[activeShip]->isChasing = true;//tell the ship its new state is chasing 
				playerFleet[activeShip]->activeTarget = ship;//make the enemy its active target
				activeShip = -1;//set the active ship (the one we're commanding) to a null value
			}
			break;
		}
		else {
			//if our mouse isn't hovering over a ship theres not much we can do
			isMouseOverShip = false;
		}
	}


	//if the player has clicked while giving orders, and not hovering over an enemy ship
	if (playerController.has3DClick) {

		if (activeShip >= 0) { //shouldn't need this here, but ensure its a valid ship index (being safe is good)
			//get the point where the player clicked in 3d space
			shipWaypoint = playerController.getClickPos();
			//tell the active ship to move to the click pos
			playerFleet[activeShip]->MoveToDestination(shipWaypoint);
			isGivingOrders = false;//our orders are complete
			activeShip = -1;//set to null index
		}
	}
}


void SceneGameplay::UpdatePlayerFleet(const float deltaTime)
{

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

	//if a player ship is being commanded, tell that ship to display its range sphere
	if (activeShip >= 0) {
		isGivingOrders = true;
		playerFleet[activeShip]->displayRange = true;
		//if the active ship is also moving, draw the path line so player knows where its going
		if (playerFleet[activeShip]->isMoving) {
			pathLine.RecalculateLine(playerFleet[activeShip]->transform.getPos(), playerFleet[activeShip]->destination);
		}
	}
}

void SceneGameplay::RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime)
{
	// Check if the ship is not moving
	if (!ship->isMoving) {

		// If the rotation timer is less than or equal to 1, find the closest enemy
		if (rotationTimer <= 1) {
			ship->FindClosestEnemy(targetShip);
			rotationTimer += deltaTime;  // Increment the rotation timer by the elapsed time
		}
		else {
			rotationTimer = 0;  // Reset the rotation timer if it exceeds 1
		}

		// If the current target index matches the target ship's index
		if (ship->currentTargetIndex == targetShip->shipIndex) {
			Vec3 targetDirection = targetShip->transform.getPos() - ship->transform.getPos();
			Quaternion targetQuad = QMath::lookAt(targetDirection, UP);
			ship->transform.setOrientation(targetQuad);
			ship->initialDirection = targetDirection;
		}

		// If the ship is switching targets
		if (ship->isSwitchingTarget) {
			ship->slerpT = ship->slerpT + deltaTime;  // Increment the slerpT value by the elapsed time
			if (ship->slerpT >= 1) {
				ship->closestEnemy = ship->potentialTarget;  // Update the closest enemy to the potential target
				ship->currentTargetIndex = ship->closestEnemy->shipIndex;  // Update the current target index
				ship->isSwitchingTarget = false;  // Reset the switching target flag
				ship->slerpT = 0;  // Reset the slerpT value
			}
		}
	}
}



void SceneGameplay::UpdateEnemyFleet(const float deltaTime)
{
	for (int i = 0; i < enemyFleet.size(); i++) { //loop through all the enemy ships

		if (enemyFleet[i]->deleteMe) {//if any of them have set a flag telling the scene to delete them
			DestroyEnemy(i);//call the destroy enemy function (in this scene)
		}
		else {
			enemyFleet[i]->Update(deltaTime); //else, you can safely update the enemy ships

			//if they collided with the planet, apply damage to it and destroy the enenmy who did it 
			if (COLLISION::SphereSphereCollisionDetected(enemyFleet[i]->collisionSphere, planet.collisionSphere)) {
				planet.takeDamage(enemyFleet[i]->damage);
				audioManager->PlaySound2D("Planet_Hit");
				DestroyEnemy(i);
			}


		}
	}
}



void SceneGameplay::DestroyEnemy(int index)
{
	score++; //increment the score variable

	Vec3 explosionPos = enemyFleet[index]->transform.getPos(); //get the last position of the enemy ship to run the explosion
	bool explosionAvailable = false;//assume there are no explosions available
	for (Explosion* explosion : explosions) { //loop through all explosions
		if (!explosion->animComplete) { //if one isn't running its animation
			explosionAvailable = true;//tell the program an explosion is avialable
			explosion->setPos(explosionPos);//set the explosion position 
			explosion->ResetExplosion(computeReset);//set and arm the explosion ()onexplode is then called from reset
		}
	}

	if (!explosionAvailable) {
		//if there are no explosions availabel then cretae a new one
		explosions.push_back(new Explosion());
		if (explosions[explosions.size() - 1]->OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh, &debrisModel) == false) {
			std::cout << "Explosion failed ... we have a problem\n";
		}
		//use the back eleement (explosion we just created) to set the pause and explode
		explosions[explosions.size() - 1]->setPos(explosionPos);
		explosions[explosions.size() - 1]->ResetExplosion(computeReset);
	}

	//free up the memory used by the destroyed enemy and delete it
	enemyFleet[index]->OnDestroy();
	delete enemyFleet[index];
	enemyFleet[index] = nullptr;
	enemyFleet.erase(std::remove(enemyFleet.begin(), enemyFleet.end(), nullptr), enemyFleet.end());
}

void SceneGameplay::GameOver()
{
	SaveStats(); //save player stats to the leaderboard
	audioManager->PlaySound2D("Game_Over");//play the game over sound
	gameOverBool = true;//"flagspatcher" to tell scenemanager the game is over
}


void SceneGameplay::SaveStats() {
	// Open a file in append mode
	std::ofstream outFile("Leaderboard.txt", std::ios::app);

	// Check if the file is open
	if (outFile.is_open()) {
		// Write the data to the file
		outFile << timeElapsed << " " << score << " " << nameEntry << "\n";
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


	planeModel = Model("Plane.obj", std::vector<std::string>{"Grid2.png"});
	if (planeModel.OnCreate() == false) {
		printf("Model failed to load");
	}
}

void SceneGameplay::createActors()
{

	//construct the starting enemies
	for (int i = 0; i < startingFleetSize; i++) {
		int numShips = startingFleetSize;
		float radius = PLANET_RADIUS + 25;
		float angle = 2.0f * M_PI * i / numShips;										// Calculate the angle for each ship
		float x = radius * cos(angle);													// Calculate x position
		float z = radius * sin(angle);													// Calculate z position

		enemyFleet.push_back(new EnemyShip(Vec3(x, 0, z), &enemyShipModel, ENEMY_DEFAULT_HEALTH));
	}

	//create the starting enemies
	for (EnemyShip* ship : enemyFleet) {
		ship->SetAudioManager(audioManager);
		ship->OnCreate();

		ship->exhaustTrail.OnCreate(&playerController.camera, loadVertsToBuffer, particleMesh);
		ship->setIndex(enemyIndex);
		enemyIndex++;
	}

	//construct teh player fleet
	for (int i = 0; i < startingFleetSize; i++) {
		playerFleet.push_back(new FriendlyShip(&friendlyShipModel, &bulletModel));
	}

	//spawn the ships in a radius around the planet

	int numShips = playerFleet.size();
	float radius = PLANET_RADIUS + 5;

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
		playerFleet[i]->rangeSphere = &sphereModel;
	}

	//create the planet object
	planet = Planet(PLANET_RADIUS, PLANET_HEALTH, &sphereModel, ORIGIN);
	planet.OnCreate();


	//create all the objects the user uses to interface with the world
	selectionSphere = Actor(Transform(), &sphereModel);
	enemySelectionSphere = Actor(Transform(), &sphereModel);
	cursorSphere = Actor(Transform(), &sphereModel);
	cursorSphere.meshColour = ORANGE;
	selectionSphere.transform.setPos(0, 0, -0.5);//give a little offset as the ship's origin is not at their centre
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

void SceneGameplay::createPlayerController()
{
	playerController.CreateGrid(&planeModel); //create the grid the player uses for 3d clicks
	playerController.setPlayerBounds(planet.collisionSphere->r + 10, PLAYER_OUTER_BOUNDS); //sets the physical boundaries of the world the player can move to, outerbounds are defined in constant.h and the innerbounds are the planet with a little buffer
	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}
}