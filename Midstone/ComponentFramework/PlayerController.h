#pragma once
#include "Camera.h"
#include "Trackball.h"
#include "Transform.h"
#include <QMath.h>
#include "Actor.h"
#include "Shader.h"
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
		float planeDepth = 0;

	public:
		Transform transform; //temporary public
		Camera camera;
		bool has3DClick = false;
		bool hasDQLine = false;
		bool OnCreate();
		void OnDestroy();
		void handleEvents(const SDL_Event& sdlEvent);
		void Update(const float deltaTime);
		void Render(Shader* shader) const;
		Vec3 getClickPos();//returns the position of a 3d click
		Vec3 get3DClickCoords(float sdl_X, float sdl_Y);
		DualQuat getLine() { hasDQLine = false;  return line2; };
		
};



