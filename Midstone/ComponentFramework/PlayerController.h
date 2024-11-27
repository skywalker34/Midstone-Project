#pragma once
#include "Camera.h"
#include "Trackball.h"
#include "Transform.h"
#include <QMath.h>
#include "Actor.h"
#include "Shader.h"
#include "Model.h"
using namespace MATH;

union SDL_Event;



class PlayerController
{
	private:
		const float CAMERA_SPEED = 0.5f;
		float scrollSpeed = 2; //rate of speed which the player can scroll the wheel

		Actor clickGrid; //the grid the player can interact with
		Trackball trackball; //trackball to allow camera rot

		DualQuat line; //ray/line for player-world interaction
		Vec3 clickPos; //position (in 3d world space) the player has clicked

		Vec3 forwardVector; //which way is the player pointing
		
		float planeDepth = 50.0f;//teh z-coordinate of teh click grid in camera space, statring value: 50

		//constants
		const float PLANE_MAX_DEPTH = 400.0f; //as far out you can scroll the plane
		const float PLANE_MIN_DEPTH = 1.0f; //as close as you can bring the plane
		const Vec3 STARTING_POS = Vec3(0.0f, 0.0f, 85.0f); //starting position

		Vec3 mouseHoverPos2D; //position your mouse is at in 2d space

		

	public:
		PlayerController();


		Vec3 mouseHoverPos3D;//the 3d position the mouse is hovering over

		Sphere innerBounds; //the inner bounds the player can move to
		Sphere outerBounds;//the outer bounds the player cna move to

		Transform transform;//positoon of the player in world space
		Camera camera; //the camera for rendering the 3d scene

		bool has3DClick = false; //tells the scene when the player has clicked
		bool hasDQLine = false; //tells the scene when it should pull the DQ line 
		bool hasCanceledOrder = false; //tells the scene the player has cancelec their clicking order



		/// <summary>
		/// Initializes the necessary resources or settings when creating the object.
		/// </summary>
		/// <returns>Returns true if the creation is successful, otherwise false.</returns>
		bool OnCreate();

		/// <summary>
		/// Cleans up resources or performs any necessary actions before destroying the object.
		/// </summary>
		void OnDestroy();

		/// <summary>
		/// Handles events such as input events from SDL.
		/// </summary>
		/// <param name="sdlEvent">The SDL event to handle.</param>
		void handleEvents(const SDL_Event& sdlEvent);

		/// <summary>
		/// Updates the state of the object based on the elapsed time.
		/// </summary>
		/// <param name="deltaTime">The time elapsed since the last update.</param>
		void Update(const float deltaTime);

		/// <summary>
		/// Renders the object using the specified shader.
		/// </summary>
		/// <param name="shader">The shader used for rendering.</param>
		void Render(Shader* shader) const;

		/// <summary>
		/// Creates a grid using the provided model.
		/// </summary>
		/// <param name="model_">The model used to create the grid.</param>
		void CreateGrid(Model* model_);

		/// <summary>
		/// Returns the position of a 3D click.
		/// </summary>
		/// <returns>The 3D coordinates of the click position.</returns>
		Vec3 getClickPos();

		/// <summary>
		/// Converts 2D screen coordinates to 3D coordinates.
		/// </summary>
		/// <param name="sdl_X">The x-coordinate on the screen.</param>
		/// <param name="sdl_Y">The y-coordinate on the screen.</param>
		/// <returns>The 3D coordinates corresponding to the 2D screen coordinates.</returns>
		Vec3 get3DClickCoords(float sdl_X, float sdl_Y);

		/// <summary>
		/// Calculates a line based on current data.
		/// </summary>
		void calculateLine();

		/// <summary>
		/// Retrieves the current line as a Dual Quaternion.
		/// </summary>
		/// <returns>The current line represented as a Dual Quaternion.</returns>
		DualQuat getLine() { hasDQLine = false;  return line; };

		/// <summary>
		/// Sets the player's bounds using the specified inner and outer distances.
		/// </summary>
		/// <param name="innerDis">The inner boundary distance.</param>
		/// <param name="outerDis">The outer boundary distance.</param>
		void setPlayerBounds(float innerDis, float outerDis);


		
};



