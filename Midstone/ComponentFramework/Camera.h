#pragma once
#include <Vector.h>
#include <Matrix.h>
#include <Quaternion.h>
#include <DQMath.h>
#include "Transform.h"
#include "SkyBox.h"
#include "Trackball.h"
#include "Constants.h"
using namespace MATH;
using namespace MATHEX;

/// <summary>
/// A camera is how we see the world, is simply a container for a projection matrix, view dual quaternion and skybox
/// </summary>
class Camera {
private:
	
	Matrix4 projection; //projection matrix
	DualQuat viewDq; //view dual quaternion instead of a matrix4 (gives us more freedom like slerps)

	bool orbitMode = false; //bool whetehr we want the camera to orbit the origin or fly freely


	SkyBox* skybox; //the skybox for the scene

	Transform transform; //psotion,orientation of the camera in one conevnient package
public:
	
	/// <summary>
/// Default constructor for the Camera class.
/// </summary>
	Camera();

	/// <summary>
	/// Destructor for the Camera class.
	/// </summary>
	~Camera();

	/// <summary>
	/// Sets the view of the camera using the specified orientation and position.
	/// </summary>
	/// <param name="orientation_">The orientation of the camera.</param>
	/// <param name="position_">The position of the camera.</param>
	void SetView(const Quaternion& orientation_, const Vec3& position_);

	/// <summary>
	/// Sets the view of the camera using the specified transform.
	/// </summary>
	/// <param name="t_">The transform of the camera.</param>
	void SetView(const Transform t_);

	/// <summary>
	/// Returns the view dq as a matrix4
	/// </summary>
	/// <returns>Returns the view matrix for the camera.</returns>
	Matrix4 GetViewMatrix() const;

	/// <summary>
	/// Gets the projection matrix of the camera.
	/// </summary>
	/// <returns>Returns the projection matrix of the camera.</returns>
	Matrix4 GetProjectionMatrix() const;

	/// <summary>
	/// Gets the view dual quaternion of the camera.
	/// </summary>
	/// <returns>Returns the view dual quaternion of the camera.</returns>
	DualQuat GetViewDQuaternion() const;

	/// <summary>
	/// Toggles the orbit mode of the camera.
	/// </summary>
	void toggleOrbitMode() {
		orbitMode = !orbitMode;
	}

	/// <summary>
	/// Initializes the camera.
	/// </summary>
	/// <returns>Returns true if the initialization is successful, otherwise false.</returns>
	bool OnCreate();

	/// <summary>
	/// Cleans up resources before destroying the camera object.
	/// </summary>
	void OnDestroy();

	/// <summary>
	/// Renders the skybox.
	/// </summary>
	void RenderSkyBox() const;

	/// <summary>
	/// Gets the texture ID of the skybox.
	/// </summary>
	/// <returns>Returns the texture ID of the skybox.</returns>
	GLuint GetSkyBoxTextureID() const {
		return skybox->GetTextureID();
	}

	
};

