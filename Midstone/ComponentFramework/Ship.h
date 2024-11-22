#pragma once
#include "Actor.h"
#include "Body.h"
#include "ExhaustTrail.h"
#include "irrKlang.h"

using namespace irrklang;
class Body;

class Ship :
    public Actor
{
    public:
        int health;
        int damage;
        float speed = 1;

        Body* body = nullptr;
        Matrix4 shipModelMatrix;

        ExhaustTrail exhaustTrail;

        ISoundEngine* SoundEngine = createIrrKlangDevice(); //Sound Device For Shooting Sounds
        ISoundEngine* SoundEngineFlying = createIrrKlangDevice(); // Sound Device for flying

        Ship() {}
        Ship(int health_, int damage_, float speed_);
        bool OnCreate();
        //virtual void moveToPosition();
        ~Ship();


};

