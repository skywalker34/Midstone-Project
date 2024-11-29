#pragma once
#include "irrKlang.h"
#include <string>
#include <vector>
#include "Vector.h"

using namespace irrklang;
using namespace MATH;

class AudioManager
{
    // Multi sounds play one sound randomly from these vectors when called
    std::vector<std::string> planetHit; // Sounds played when a planet is hit (2D)
    std::vector<std::string> shipShooting; // Sounds played when a ship shoots (3D)
    std::vector<std::string> enemyExplosion; // Sounds played when an enemy explodes (3D)
    std::string enemyHit; // Sound played when an enemy is hit (3D)
    // Single sounds
    std::string gameOver; // Sound played when the game is over (2D)
    std::string shipSelected; // Sound played when a ship is selected (2D)
    std::string rocketLoop; // Looping sound played for a ship moving (3D)

    // Vector to store looping sounds
    std::vector<ISound*> soundLoops;

public:
    /// <summary>
    /// Constructor for AudioManager.
    /// </summary>
    AudioManager();

    /// <summary>
    /// Sound engine for audio throughout all scenes.
    /// </summary>
    ISoundEngine* SoundEngine = nullptr;

    /// <summary>
    /// Initializes the audio manager.
    /// </summary>
    /// <returns>True if the audio manager was successfully created.</returns>
    bool OnCreate();

    /// <summary>
    /// Destroys the audio manager and frees resources.
    /// </summary>
    void OnDestroy();

    /// <summary>
    /// Plays a 2D sound.
    /// </summary>
    /// <param name="name">The name of the sound to play.</param>
    void PlaySound2D(const std::string& name);

    /// <summary>
    /// Plays a 3D sound at a specified position.
    /// </summary>
    /// <param name="name">The name of the sound to play.</param>
    /// <param name="pos">The position to play the sound at.</param>
    void PlaySound3D(const std::string& name, const Vec3& pos);

    /// <summary>
    /// Plays a 3D looping sound and returns its index in the vector so it can be stopped later.
    /// </summary>
    /// <param name="name">The name of the sound to play.</param>
    /// <param name="pos">The position to play the sound at.</param>
    /// <returns>The index of the looping sound in the soundLoops vector.</returns>
    int PlaySound3DLooped(const std::string& name, const Vec3& pos);

    /// <summary>
    /// Stops a 3D looping sound given its index.
    /// </summary>
    /// <param name="index">The index of the sound to stop.</param>
    void StopSound3DLooped(int index);
};
