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
	Transform transform;
	Model* model;

	Vec4 meshColour = Vec4(1,1,1,1);
	
	Sphere* collisionSphere = nullptr;

	Actor();
	virtual bool OnCreate();
	virtual void OnDestroy();

	Actor(Transform transform_, Model* model_);
	void Render(Shader* shader) const;
	// Functions:
	// render

    ~Actor();
};

