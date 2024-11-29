#pragma once
#include "Actor.h"
#include "Model.h"
/// <summary>
/// The planet is the main place of interest the player is trying to protect and the enemies are trying to destroy
/// </summary>
class Planet : 
    public Actor
{
    

   

   

    //inherited from actor:
        //Transform transform;
        //Model* model;

    int health; //damage planet can take before it is destroyed
    float radius; //planet's radius (used for scale and collision)
    Transform atmosphere; //transform for the planet's atmosphere mesh/texture

public:
    /// <summary>
/// Default constructor
/// Initializes a planet with default values.
/// </summary>
    Planet();

    /// <summary>
    /// Parameterized constructor
    /// Initializes a planet with a specific radius, health, model, and position.
    /// </summary>
    /// <param name="radius_">The radius of the planet.</param>
    /// <param name="health_">The health of the planet.</param>
    /// <param name="model_">Pointer to the model associated with the planet.</param>
    /// <param name="pos">The position of the planet.</param>
    Planet(float radius_, int health_, Model* model_, Vec3 pos);

    /// <summary>
    /// Initializes the planet and sets up necessary resources.
    /// </summary>
    /// <returns>True if creation was successful, false otherwise.</returns>
    bool OnCreate();

    /// <summary>
    /// Cleans up resources used by the planet.
    /// </summary>
    void OnDestroy();

    /// <summary>
    /// Updates the planet's state based on the elapsed time.
    /// </summary>
    /// <param name="deltaTime">The time elapsed since the last update.</param>
    void Update(float deltaTime);

    /// <summary>
    /// Renders the planet using the given shader.
    /// </summary>
    /// <param name="shader">The shader to use for rendering.</param>
    void Render(Shader* shader) const;

    /// <summary>
    /// Inflicts damage on the planet, reducing its health.
    /// </summary>
    /// <param name="damage">The amount of damage to inflict.</param>
    void takeDamage(int damage);

    /// <summary>
    /// Gets the current health of the planet.
    /// </summary>
    /// <returns>The current health of the planet.</returns>
    int GetHealth() { return health; }

};

