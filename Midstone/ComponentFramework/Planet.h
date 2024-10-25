#pragma once
#include "Actor.h"
#include "Model.h"

class Planet : 
    public Actor
{
    //The planet is the main place of interest the player is trying to protect and the enemies are trying to destroy

    //Variables/components:
    //health

    //Functions:
    //takeDamage(damage)

    //inherited from actor:
        //Transform transform;
        //Model* model;

    int health;
    float radius; 

public:
    Planet();
    Planet(float radius_, int health_, Model* model_, Vec3 pos);
    bool OnCreate();
    void OnDestroy();
    void Update(float deltaTime);
    void Render(Shader* shader) const; 

    void takeDame(int damage);
};

