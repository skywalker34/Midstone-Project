#pragma once
#include "irrKlang.h"
#include <string>
#include <vector>
#include "Vector.h"

using namespace irrklang;
using namespace MATH;

class AudioManager
{
    // Multi sounds play one sound randomly from this vector when called
    std::vector<std::string> planetHit; // 2D
    std::vector<std::string> shipShooting; // 3D
    std::vector<std::string> enemyExplosion; // 3D
    std::string enemyHit; // 3D
    // Single Sounds
    std::string gameOver; // 2D
    std::string shipSelected; // 2D
    std::string rocketLoop; // 3D

    std::vector<ISound*> soundLoops;

public:
    AudioManager();
    ISoundEngine* SoundEngine = nullptr; // For Audio Throughout All scenes
    bool OnCreate();
    void OnDestroy();

    void PlaySound2D(const std::string& name);
    void PlaySound3D(const std::string& name, const Vec3& pos);

    // Returns the index of the sound in the std::vector so the loop can later be called to stop
    int PlaySound3DLooped(const std::string& name, const Vec3& pos);
    void StopSound3DLooped(int index);
};
