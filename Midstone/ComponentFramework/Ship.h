#pragma once
#include "Actor.h"
#include "Body.h"
#include "ExhaustTrail.h"
#include "irrKlang.h"
#include "AudioManager.h"

using namespace irrklang;
class Body;

class Ship :
    public Actor
{
    public:
        int health;
        int damage;
        float speed = 1;

        int rocketSoundIndex;

        Body* body = nullptr;
        Matrix4 shipModelMatrix;

        AudioManager* audioManager;

        ExhaustTrail exhaustTrail;

        Ship() {}
        Ship(int health_, int damage_, float speed_);
        bool OnCreate();
        void SetAudioManager(AudioManager* audioManager_) { audioManager = audioManager_; }
        //virtual void moveToPosition();
        ~Ship();


};

