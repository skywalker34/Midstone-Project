#pragma once
#include "Model.h"
#include "Transform.h"
#include "sphere.h"
using namespace MATHEX;
class Actor
{
public:	
	//actor is any object in the scene with a visual repersentation
	//Known actors: Planet, Ships(Friendlyship, enemyship)
	//MATHEX::Sphere collider;
	Transform transform;
	Model model;


	
	Sphere* collisionSphere = nullptr;

	Actor();
	virtual bool OnCreate();

	Actor(Transform transform_, Model model_);
	void Render(Shader* shader) const;
	// Functions:
	// render

    ~Actor();
};

