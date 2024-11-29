#include "EnemySpawner.h"


EnemySpawner::EnemySpawner()
{
}

EnemySpawner::EnemySpawner(float distance_, int spawnRate_, int spawnMax_)
{
	distanceFromOrigin = distance_;//how far the spawner should be ath the origin
	spawnRate = spawnRate_;//how often this spawner spawns units
	spawnMax = spawnMax_; //how many units this spawner can spawn at this pos

	srand(static_cast<unsigned int>(time(0))); //reroll the seed for random number generation
	
	position = Vec3(fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation); //create a random vector
	position = VMath::normalize(position); //normalize the position (make length equal to 1)
	position *= distanceFromOrigin; //lengthen the magnitude back to our desired distance
	
	
}

void EnemySpawner::Update(const float deltaTime)
{
	elapsedTime += deltaTime;//add time to the spawners internal clock
	if ((int)elapsedTime % spawnRate == 0.0f) { //if we've hit a spawn rate interval

		elapsedTime = 1; //set the time back down to 1
		canSpawn = true; //tell teh scene a ship should be spawned
		spawnCount++; //increment the variable that keeps track of how many enemies this spawner ahs spawned
	}
	else {
		//otherwise we can't spawn an enemy
		canSpawn = false;
	}

	if (spawnCount == spawnMax) { //once its spawn count has maxed out this spawner is effectively "destroyed"
		//however since it would be a wast e to destroy it and create a new one, reposition it instead
		RePosition();
	}

}

void EnemySpawner::RePosition()
{
	//reset all the variables 
	spawnCount = 0;

	position = Vec3(fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation); //create a random vector
	position = VMath::normalize(position); //normalize the position (make length equal to 1)
	position *= distanceFromOrigin; //lengthen the magnitude back to our desired distance
}
