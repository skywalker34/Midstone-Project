#pragma once
#include "Actor.h"
#include "Body.h"
#include "ExhaustTrail.h"
#include "irrKlang.h"
#include "AudioManager.h"

using namespace irrklang;
class Body; //foward dec

/// <summary>
/// Base clase for both the friendly and enemy ships
/// </summary>
class Ship :
    public Actor
{
    public:
        int health; //ammount of damage the ship can take before being destroyed
        int damage; //amount of damage the ship applies to other ships/objects
        float speed = 1; //how fast the ship can move in space

        int rocketSoundIndex; //holds the index of the ship's active rocket sound loop in the audio manager (-1 if not playing)

        Body* body = nullptr; //physics body
      

        AudioManager* audioManager; //reference to the audio manager so teh ship can call it to play sounds

        ExhaustTrail exhaustTrail; //exhaust trail particle system to animate the ship's mvements
        
        /// <summary>
        /// Default constructor, don't use
        /// </summary>
        Ship() {}
        
 
        /// <summary>
        /// Sets the audio manager taht will handle the ship's sounds
        /// </summary>
        /// <param name="audioManager_"></param> Reference to the audio manager loaded in the scene which holds the sound device and all sounds
        void SetAudioManager(AudioManager* audioManager_) { audioManager = audioManager_; }

        /// <summary>
        /// Destructor
        /// </summary>
        ~Ship();


};

