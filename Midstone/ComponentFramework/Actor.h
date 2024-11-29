#pragma once
#include "Model.h"
#include "Transform.h"
#include "sphere.h"
using namespace MATHEX;

/// <summary>
/// actor is any object in the scene with a visual repersentation Known actors: Planet, Ships(Friendlyship, enemyship)
/// </summary>
class Actor
{
public:	
	
	Transform transform; // holds the positional, rotational and scaler values of the object in world space
	Model* model; // a reference to a model loaded in teh scene (flyweight)

	Vec4 meshColour = Vec4(1,1,1,1); //colour to render the mesh at if no shader is set
	
	Sphere* collisionSphere = nullptr; //sphere to handle physics collisions

	/// <summary>
	/// Default constructor (DO NOT USE)
	/// Initializes an actor with default values.
	/// </summary>
	Actor();

	/// <summary>
	/// Called when the actor is created.
	/// Should be overridden by derived classes to perform initialization.
	/// </summary>
	/// <returns>True if creation was successful, false otherwise.</returns>
	virtual bool OnCreate();

	/// <summary>
	/// Called when the actor is destroyed.
	/// Should be overridden by derived classes to perform cleanup.
	/// </summary>
	virtual void OnDestroy();

	/// <summary>
	/// Parameterized constructor
	/// Initializes an actor with a specific transform and model.
	/// </summary>
	/// <param name="transform_">The transform to initialize the actor with.</param>
	/// <param name="model_">The model associated with this actor.</param>
	Actor(Transform transform_, Model* model_);

	/// <summary>
	/// Renders the actor using the given shader.
	/// </summary>
	/// <param name="shader">The shader to use for rendering.</param>
	void Render(Shader* shader) const;

	/// <summary>
	/// Destructor
	/// Ensures cleanup of resources.
	/// </summary>
	~Actor();

};

