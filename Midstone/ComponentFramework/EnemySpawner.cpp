#include "EnemySpawner.h"

EnemySpawner::EnemySpawner()
{
}

EnemySpawner::EnemySpawner(float distance_, int spawnRate_)
{
	distanceFromOrigin = distance_;
	spawnRate = spawnRate_;

	position = Vec3(fmod(rand(), spawnVariation), fmod(rand(), spawnVariation), fmod(rand(), spawnVariation)); //create a random vector
	position = VMath::normalize(position); //normalize the position (make length equal to 1)
	position *= distanceFromOrigin; //lengthen the magnitude back to our desired distance
}

void EnemySpawner::Update(const float deltaTime)
{
	elapsedTime += deltaTime;
	if ((int)elapsedTime % spawnRate == 0.0f) {

		elapsedTime = 1;
		canSpawn = true;
	}
	else {
		canSpawn = false;
	}
}
