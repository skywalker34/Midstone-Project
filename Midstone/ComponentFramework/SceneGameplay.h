#ifndef SCENE3G_H
#define SCENE3G_H
#include "Scene.h"
#include "Vector.h"
#include "PlayerController.h"
#include "Actor.h"
#include "FriendlyShip.h"
#include "EnemyShip.h"
#include <Matrix.h>
#include "Collision.h"
#include "Vector.h"
#include "Planet.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "Line.h"
#include "EnemySpawner.h"
#include "Explosion.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Mesh;
class Shader;
class Bullet;
class Model;
class ComputeShader;
class AudioManager;

class SceneGameplay : public Scene {
private:

	//SHADERS
	Shader* shader; //default shader, renders a mesh a certain colour without lighting (use with wireframe mode to achieve 3d)
	Shader* bulletShader; //uses fresnel effect to make a pseudo glowinf effect
	Shader* planetShader; //uses fresnel effect as well as a specular mask to give a planet a matte atmosphere effect
	Shader* friendlyShipShader; //uses an RGB colour mask and phong for colour customization and lightinh
	Shader* gridShader; //uses texture and alpha blend for see-through grid
	Shader* selectionShader; //uses dot product to render a 2d circle from a 3d sphere
	Shader* particleShader = nullptr; //uses a vertex distance between itself and the orifgin for colour gradient effect

	//COMPUTE SHADERS (for paticle systems)
	ComputeShader* computeShader = nullptr; //applies a force to particles within a conical direction for jet streams
	ComputeShader* computeExplosion = nullptr; //applies a random force outward from a point, creating an explosion effect
	ComputeShader* computeReset = nullptr; //reset compute shader particles to their origin
	Shader* loadVertsToBuffer = nullptr; //not a compute shader but neccessary for their ussage. isolate the vertex pos on the GPU fro the buffer and write it to a unique buffer used by compute shaders
	Shader* lineShader = nullptr; //shader to render the lines
	
	Mesh* particleMesh; //mesh used for particle systems  (edited in notepad to be a couple thousand of verts stacked on the origin)


	//MODELS
	Model friendlyShipModel;
	Model enemyShipModel;
	Model bulletModel;
	Model sphereModel;
	Model planeModel;
	Model debris;

	
	

	//UX ACTORS
	Actor selectionSphere;
	Actor cursorSphere;
	Actor enemySelectionSphere;
	
	//SCENE ACTORS
	Planet planet;
	std::vector<FriendlyShip*> playerFleet;
	std::vector<EnemyShip*> enemyFleet;

	AudioManager* audioManager;
	
	Vec3 lightPos = Vec3(0.0f, 0.0f, 150.0f);


	PlayerController playerController;



	Vec3 shipWaypoint;

	

	
	Line pathLine = Line(Vec3(0, 0, 0), Vec3(1, 1, 1));
	Line testLine = Line(Vec3(0, 0, 0), Vec3(100, 100, 100));

	

	std::vector<Explosion*> explosions;
	std::vector<EnemySpawner> enemyFleetSpawners;


	int enemySpawnerCount = 1;

	int activeShip = -1;
	int startingFleetSize = 5;
	int enemyIndex = 0;
	
	int startingExplosions = 10; //we're going to create 10 explosions and recycle them whenever an enemy blows up.

	float timeElapsed;
	float rotationTimer = 0;

	int score = 0;
	const int ENEMY_DEFAULT_HEALTH = 5;

	bool drawInWireMode;
	bool isGameRunning = true;
	bool isGivingOrders = false;
	bool isMouseOverShip = false;

	Window* window;

	float volumeSlider = 0.4f;
	float sfxSlider = 0.4f;

	ImVec4 shipColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

	void SaveStats();
public:
	explicit SceneGameplay();
	SceneGameplay(Window* window_);
	virtual ~SceneGameplay();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() override;
	virtual void RenderIMGUI() override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	void SpawnEnemy(const float deltaTime);
	void PlayerControllerHandleEvents();
	void RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime);
	void PlayerOrders();

	//update functions

	void UpdatePlayerFleet(const float deltaTime);
	void UpdateEnemyFleet(const float deltaTime);


	//On create events

	void createModels();
	void createActors();
	void createShaders();
	void createPlayerController();
	void DestroyEnenmy(int index);

	void GameOver();
};


#endif // SCENE0_H#pragma once
