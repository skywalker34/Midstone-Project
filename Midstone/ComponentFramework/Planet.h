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

    int health; //damage planet can take before it is destroyed
    float radius; //planet's radius (used for scale and collision)
    Transform atmosphere; //transform for the planet's atmosphere mesh/texture

public:
    Planet();
    Planet(float radius_, int health_, Model* model_, Vec3 pos);
    bool OnCreate();
    void OnDestroy();
    void Update(float deltaTime);
    void Render(Shader* shader) const; 

    void takeDamage(int damage);

    int GetHealth() { return health; }
};

