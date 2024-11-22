#pragma once
#include "irrKlang.h"
#include "string"
#include "Vector.h"

using namespace irrklang;
using namespace MATH;

class AudioManager
{
	//Multi sounds
	std::string planetHit[1] = {"audio/PlanetGotHit.mp3"};
	std::string shipShooting[3] = { "audio/LaserShooting.mp3", "audio/DisturbedShooting.mp3", "audio/PewShoot.mp3" };
	std::string enemyExplosion[1] = { "audio/DeadExplosion.mp3" };
	std::string enemyHit = { "audio/Gothit.mp3." };
	//Single Sounds
	std::string gameOver = "audio/GameoverSound.mp3";
	std::string shipSelected = "audio/SelectionSound.mp3";
	std::string rocketLoop = "audio/RocketFlying.mp3";
	
	

public:

	ISoundEngine* SoundEngine = createIrrKlangDevice(); // For Audio Throughout All scenes
	bool OnCreate();
	void OnDestroy();

	void PlaySound2D(std::string name);

	void PlaySound3D(std::string name, Vec3 pos);
};

