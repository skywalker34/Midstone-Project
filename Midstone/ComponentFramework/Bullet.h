#pragma once
#include "Actor.h"
#include "Constants.h"
class Bullet :
    public Actor
{
    //Bullet is an actor that is spawned by ships
    //Bullets will fly along a straight vector (uneffeted by physics) 

    //Variable/Components
    //Damage
    //Speed (may be handled elsewhere)
    //Model

    float speed;
    Vec3 forward;
    Vec3 initialPos;
    const float maxRange = 10;//how far the bullet can travel before it deletes itself
    
    Vec4 color = ORANGE;

    //Functions
    //OnCreate();
    //OnDestroy()


    

public:
    bool deleteMe = false;
    Bullet();
    Bullet(Transform spawnPos, float speed_, Vec3 forward_);
    bool OnCreate();
    void OnDestroy();
    void Update(const float deltaTime);
    void Render(Shader* shader) const;

};

