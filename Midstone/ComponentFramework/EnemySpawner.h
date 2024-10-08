#pragma once
#include <VMath.h>

using namespace MATH;

class EnemySpawner
{
	float elapsedTime = 1 ;
	float distanceFromOrigin;
	int spawnRate;

	

	const int spawnVariation = 15; //level of variation when generating random values

	public:
		bool canSpawn = false;
		Vec3 position; //might want to make a getter/setter instead
		EnemySpawner();
		EnemySpawner(float distance_, int spawnRate_);

		void Update(const float deltaTime);
};

