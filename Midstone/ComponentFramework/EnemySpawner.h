#pragma once
#include <VMath.h>

using namespace MATH;

class EnemySpawner
{
    float elapsedTime = 1;//time since last spawn (start at 1 for % use)
    float distanceFromOrigin; // how far this spawner should be from the world origin
    int spawnRate; //how often an enemy is spawned from this spawner

    const int spawnVariation = 15; // Level of variation when generating random values

public:
    bool canSpawn = false; //whether or not the spawner can spawn an enemy at this frame
    int spawnCount = 0; //active count of how many units this spawner has spawned
    int spawnMax; //amount of units this spawner can spawn before moving
    Vec3 position; //spawners position in world space

    /// <summary>
    /// Default constructor for the EnemySpawner class (DO NOT USE).
    /// </summary>
    EnemySpawner();

    /// <summary>
    /// Enemy spawner constructor with a specified distance, spawn rate, and maximum spawn count.
    /// </summary>
    /// <param name="distance_">The distance the enemy spawner is from the origin (exact position is calculated randomly).</param>
    /// <param name="spawnRate_">The rate (in seconds) at which enemies will spawn.</param>
    /// <param name="spawnMax_">The maximum number of enemies that can be spawned.</param>
    EnemySpawner(float distance_, int spawnRate_, int spawnMax_);

    /// <summary>
    /// Updates the spawner based on the elapsed time.
    /// </summary>
    /// <param name="deltaTime">The time elapsed since the last update.</param>
    void Update(const float deltaTime);

    /// <summary>
    /// Repositions the spawner to a new location.
    /// </summary>
    void RePosition();
};
