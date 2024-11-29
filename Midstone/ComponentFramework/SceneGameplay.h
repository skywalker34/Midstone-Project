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


/// <summary>
/// Main gameplay scene
/// </summary>
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

	//SCENE MISC

	AudioManager* audioManager;//handles the playing of sounds

	PlayerController playerController;//how the player interfaces with the game world
	std::vector<Explosion*> explosions; //list of explosion particle effects
	std::vector<EnemySpawner> enemyFleetSpawners; //list of enemy fleet spawners
	

	//MISC

	Vec3 lightPos = Vec3(0.0f, 0.0f, 150.0f); //world light pos

	

	Vec3 shipWaypoint; //holds a position for a ship to move to 

	Line pathLine = Line(Vec3(0, 0, 0), Vec3(1, 1, 1));
	Line testLine = Line(Vec3(0, 0, 0), Vec3(100, 100, 100));

	

	


	int enemySpawnerCount = 1; //amount of enenmy spawners 

	int activeShip = -1; //holds the id of the player ship that is being commanded (set to -1 if null)
	int startingFleetSize = 5; //how many ships the player/enenmy starts with
	int enemyIndex = 0; //uniquee enenmy index (increments when enemies are created)
	
	int startingExplosions = 10; //we're going to create 10 explosions and recycle them whenever an enemy blows up.

	float timeElapsed; //how much time has passed since the play session began

	float rotationTimer = 0;//timer to handle slerps 

	int score = 0; //player score (incremented when an enenmy is killed)
	const int ENEMY_DEFAULT_HEALTH = 5;

	
	bool isGameRunning = true; //used to pause the game
	bool isGivingOrders = false;//used when giving commands to ships
	bool isMouseOverShip = false;//whetehr or not the player is hovering their mouse over a ship (any ship)

	Window* window; //the window used for UI elements

	//IMGUI variables (all can be edited by the player)

	float volumeSlider = 0.4f; //music volume
	float sfxSlider = 0.4f; //sound fx volume

	ImVec4 shipColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f); //ship colour for player customization

	void SaveStats(); //save stats to leaderboard

public:

	/// <summary>
	/// Explicit default constructor
	/// Initializes the gameplay scene with default values.
	/// </summary>
	explicit SceneGameplay();

	/// <summary>
	/// Parameterized constructor
	/// Initializes the gameplay scene with a window. (Used for GUI stuff and things)
	/// </summary>
	/// <param name="window_">Pointer to the window associated with the scene.</param>
	SceneGameplay(Window* window_);

	/// <summary>
	/// Virtual destructor
	/// Ensures that resources used by the gameplay scene are cleaned up properly.
	/// </summary>
	virtual ~SceneGameplay();

	/// <summary>
	/// Initializes the gameplay scene and sets up necessary resources.
	/// </summary>
	/// <returns>True if creation was successful, false otherwise.</returns>
	virtual bool OnCreate() override;

	/// <summary>
	/// Cleans up resources used by the gameplay scene.
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// Updates the gameplay scene's state based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	virtual void Update(const float deltaTime) override;

	/// <summary>
	/// Renders the gameplay scene.
	/// </summary>
	virtual void Render() override;

	/// <summary>
	/// Renders ImGui elements for the gameplay scene.
	/// </summary>
	virtual void RenderIMGUI() override;

	/// <summary>
	/// Handles events for the gameplay scene.
	/// </summary>
	/// <param name="sdlEvent">The SDL event to handle.</param>
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	/// <summary>
	/// Updates the enenmy spawners and Spawns enemies in the gameplay scene based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void SpawnEnemy(const float deltaTime);

	/// <summary>
	/// Handles events triggered by the player controller
	/// </summary>
	void PlayerControllerHandleEvents();

	/// <summary>
	/// Rotates the specified friendly ship to face the target enemy ship.
	/// </summary>
	/// <param name="ship">Pointer to the friendly ship.</param>
	/// <param name="targetShip">Pointer to the target enemy ship.</param>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void RotateTowardEnemy(FriendlyShip* ship, EnemyShip* targetShip, const float deltaTime);

	/// <summary>
	/// Handles orders given to the ships by the player.
	/// </summary>
	void PlayerOrders();

	/// <summary>
	/// Updates the player fleet based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void UpdatePlayerFleet(const float deltaTime);

	/// <summary>
	/// Updates the enemy fleet based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void UpdateEnemyFleet(const float deltaTime);

	/// <summary>
	/// Creates models for the gameplay scene.
	/// </summary>
	void createModels();

	/// <summary>
	/// Creates actors for the gameplay scene.
	/// </summary>
	void createActors();

	/// <summary>
	/// Creates shaders for the gameplay scene.
	/// </summary>
	void createShaders();

	/// <summary>
	/// Creates the player controller for the gameplay scene.
	/// </summary>
	void createPlayerController();

	/// <summary>
	/// Destroys the enemy ship at the specified index.
	/// </summary>
	/// <param name="index">The index of the enemy ship to destroy.</param>
	void DestroyEnemy(int index);

	/// <summary>
	/// Handles the game over logic.
	/// </summary>
	void GameOver();

};


#endif // SCENE0_H#pragma once
