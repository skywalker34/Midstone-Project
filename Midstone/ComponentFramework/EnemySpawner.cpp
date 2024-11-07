#include "EnemySpawner.h"


EnemySpawner::EnemySpawner()
{
}

EnemySpawner::EnemySpawner(float distance_, int spawnRate_, int spawnMax_)
{
	distanceFromOrigin = distance_;
	spawnRate = spawnRate_;
	spawnMax = spawnMax_;

	srand(static_cast<unsigned int>(time(0))); //reroll the seed for random number generation
	
	position = Vec3(fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation); //create a random vector
	position = VMath::normalize(position); //normalize the position (make length equal to 1)
	position *= distanceFromOrigin; //lengthen the magnitude back to our desired distance
	
	
}

void EnemySpawner::Update(const float deltaTime)
{
	elapsedTime += deltaTime;
	if ((int)elapsedTime % spawnRate == 0.0f) {

		elapsedTime = 1;
		canSpawn = true;
		spawnCount++;
	}
	else {
		canSpawn = false;
	}

	if (spawnCount == spawnMax) {
		RePosition();
	}

}

void EnemySpawner::RePosition()
{

	spawnCount = 0;

	position = Vec3(fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation, fmod(rand(), 2 * spawnVariation) - spawnVariation); //create a random vector
	position = VMath::normalize(position); //normalize the position (make length equal to 1)
	position *= distanceFromOrigin; //lengthen the magnitude back to our desired distance
}
