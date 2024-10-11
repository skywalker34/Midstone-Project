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

		/// <summary>
		/// Enemys pawner constructor with a distance and spawn rate.
		/// </summary>
		/// <param name="distance_"></param>
		/// The distance the enemy spawner is from the origin (exact position is calculated randomluy
		/// <param name="spawnRate_"></param>
		/// the rate (in seconds) at which the enemy will spawn
		EnemySpawner(float distance_, int spawnRate_);

		void Update(const float deltaTime);
};

