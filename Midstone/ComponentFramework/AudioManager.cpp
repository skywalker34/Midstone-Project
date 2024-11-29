#include "AudioManager.h"
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time

AudioManager::AudioManager()
{
    // Initialize the sound arrays
    planetHit = { "audio/PlanetGotHit.mp3" };
    shipShooting = { "audio/LaserShooting.mp3", "audio/DisturbedShooting.mp3", "audio/PewShoot.mp3" };
    enemyExplosion = { "audio/DeadExplosion.mp3" };
    enemyHit = "audio/Gothit.mp3.";
    gameOver = "audio/GameoverSound.mp3";
    shipSelected = "audio/SelectionSound.mp3";
    rocketLoop = "audio/RocketFlying.mp3";
   
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

bool AudioManager::OnCreate()
{
    SoundEngine = createIrrKlangDevice();
    if (!SoundEngine) {
        return false; // Error starting up the engine
    }

    SoundEngine->setSoundVolume(0.4f);
   
    return true;
}

void AudioManager::OnDestroy()
{
    if (SoundEngine) {
        SoundEngine->drop();
    }
}

void AudioManager::PlaySound2D(const std::string& name)
{
    if (name == "Planet_Hit")
    {
        // Play the planet hit sound
        SoundEngine->play2D(planetHit[0].c_str(), false);
    }
    else if (name == "Game_Over")
    {
        //play the game over sound
        SoundEngine->play2D(gameOver.c_str(), false);
    }
    else if (name == "Ship_Selected")
    {
        //play teh ship selected sound
        SoundEngine->play2D(shipSelected.c_str(), false);
    }
    
}

void AudioManager::PlaySound3D(const std::string& name, const Vec3& pos)
{
    vec3df position(pos.x, pos.y, pos.z); // Convert Vec3 to vec3df

    if (name == "Ship_Shooting")
    {
        // Play a random shooting sound from the array
        int index = std::rand() % shipShooting.size();
        SoundEngine->play3D(shipShooting[index].c_str(), position);
    }
    else if (name == "Enemy_Explosion")
    {
        //play an enemy explosion sound
        SoundEngine->play3D(enemyExplosion[0].c_str(), position);
    }
    else if (name == "Enemy_Hit")
    {
        //plan enemy hit sound
        SoundEngine->play3D(enemyHit.c_str(), position);
    }
}

int AudioManager::PlaySound3DLooped(const std::string& name, const Vec3& pos)
{
    vec3df position(pos.x, pos.y, pos.z); // Convert Vec3 to vec3df
    ISound* sound = nullptr;

    if (name == "Rocket_Loop")
    {
        
        sound = SoundEngine->play3D(rocketLoop.c_str(), position, true, false, true);
    }

    if (sound)
    {
        soundLoops.push_back(sound); //add the sound to a list of sounds so it can be referenced again later (to stop it_
        return soundLoops.size() - 1; // Return the index of the sound in the vector
    }

    return -1; // Return -1 if the sound could not be played
}

void AudioManager::StopSound3DLooped(int index)
{
    if (index >= 0 && index < soundLoops.size() && soundLoops[index] != nullptr)
    {
        soundLoops[index]->stop();
        soundLoops[index]->drop(); // Release the sound object
        soundLoops[index] = nullptr; // Avoid dangling pointer
    }
}
