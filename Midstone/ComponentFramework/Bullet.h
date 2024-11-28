#pragma once
#include "Actor.h"
#include "Constants.h"
/// <summary>
/// Bullet is an actor that is spawned by ships Bullets will fly along a straight vector (uneffected by physics) once a bullet reaches a certain range it is deleted 
/// </summary>
class Bullet :
    public Actor
{
    

    float speed; //how fast the bullet can travel in space
    Vec3 forward; //the bullet's foward vector
    Vec3 initialPos; //initial position to spawn the bullet (the barrel of the gun essentially)
    const float maxRange = 100;//how far the bullet can travel before it deletes itself
    
    

    Model** bulletModel; //until I think of something better this is "temporary"...

    

public:
    bool deleteMe = false;//flag telling the scene to delete the bullet or not

    /// <summary>
    /// Default constructor
    /// DO NOT USE
    /// </summary>
    Bullet();

    /// <summary>
    /// Parameterized constructor
    /// Initializes a bullet with specific spawn position, speed, forward direction, and model.
    /// </summary>
    /// <param name="spawnPos">The position where the bullet spawns.</param>
    /// <param name="speed_">The speed of the bullet.</param>
    /// <param name="forward_">The forward direction vector of the bullet.</param>
    /// <param name="model_">Pointer to the model associated with the bullet.</param>
    Bullet(Transform spawnPos, float speed_, Vec3 forward_, Model** model_);

    /// <summary>
    /// Initializes the bullet and sets up necessary resources.
    /// </summary>
    /// <returns>True if creation was successful, false otherwise.</returns>
    bool OnCreate();

    /// <summary>
    /// Cleans up resources used by the bullet.
    /// </summary>
    void OnDestroy();

    /// <summary>
    /// Updates the bullet's state based on the elapsed time.
    /// </summary>
    /// <param name="deltaTime">The time elapsed since the last update.</param>
    void Update(const float deltaTime);

    /// <summary>
    /// Renders the bullet using the given shader.
    /// </summary>
    /// <param name="shader">The shader to use for rendering.</param>
    void Render(Shader* shader) const;


};

