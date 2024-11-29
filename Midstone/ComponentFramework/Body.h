#ifndef BODY_H
#define BODY_H
#include <Vector.h> 
#include "Transform.h"
using namespace MATH; 

/// <summary>
/// Allows object with this compontetn to have physics acting upon them
/// </summary>
class Body {
public:
    
// private: /// Physics stuff
	
	Vec3 vel; //physics velocity
	Vec3 accel;//physics acceleration
	Transform* transform;//a reference to the game object's transform 
	
	Vec3 angularVel;//velocity for spinning (a neat trick)
	Quaternion orientation; //orientation for calculations

	float mass; //physics mass


	
	
public:
	/// <summary>
	/// Default constructor for the Body class (DO Not use).
	/// </summary>
	Body();

	/// <summary>
	/// Constructor for the Body class with specified transform, velocity, acceleration, and mass.
	/// </summary>
	/// <param name="tran">Pointer to the transform object.</param>
	/// <param name="vel_">Initial velocity of the body.</param>
	/// <param name="acc_">Initial acceleration of the body.</param>
	/// <param name="mass_">Mass of the body.</param>
	Body(Transform* tran, Vec3 vel_, Vec3 acc_, float mass_);

	/// <summary>
	/// Destructor for the Body class.
	/// </summary>
	~Body();

	/// <summary>
	/// Initializes the body.
	/// </summary>
	/// <returns>Returns true if the initialization is successful, otherwise false.</returns>
	bool OnCreate();

	/// <summary>
	/// Cleans up resources before destroying the body object.
	/// </summary>
	void OnDestroy();

	/// <summary>
	/// Updates the state of the body based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void Update(const float deltaTime);

	/// <summary>
	/// Applies a force to the body.
	/// </summary>
	/// <param name="force">The force vector to apply.</param>
	void ApplyForce(Vec3 force);

	/// <summary>
	/// Sets the acceleration of the body.
	/// </summary>
	/// <param name="accel_">The acceleration vector to set.</param>
	void setAccel(const Vec3& accel_) { accel = accel_; }

	/// <summary>
	/// Updates the orientation of the body based on the elapsed time.
	/// </summary>
	/// <param name="deltaTime">The time elapsed since the last update.</param>
	void UpdateOrientation(float deltaTime);



	
};

#endif
