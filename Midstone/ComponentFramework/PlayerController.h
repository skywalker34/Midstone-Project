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
		
		Actor clickGrid;
		Trackball trackball;
		
		DualQuat line;
		DualQuat line2;
		Vec3 clickPos;

		Vec3 forwardVector;
		Vec3 mouseHoverPos;

		
		float planeDepth = 1;

	public:
		PlayerController();


		Vec3 hoverPos;

		Sphere innerBounds;
		Sphere outerBounds;

		Transform transform; //temporary public
		Camera camera;
		bool has3DClick = false;
		bool hasDQLine = false;
		bool OnCreate();
		void OnDestroy();
		void handleEvents(const SDL_Event& sdlEvent);
		void Update(const float deltaTime);
		void Render(Shader* shader) const;

		void CreateGrid(Model* model_);
		Vec3 getClickPos();//returns the position of a 3d click
		Vec3 get3DClickCoords(float sdl_X, float sdl_Y);
		void calculateLine();
		DualQuat getLine() { hasDQLine = false;  return line2; };

		void setPlayerBounds(float innerDis, float outerDis);

		void updateModelMatrixToFaceCamera(Transform& transform);
		
};



