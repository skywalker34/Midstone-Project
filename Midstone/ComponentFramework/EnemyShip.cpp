#include "EnemyShip.h"
#include "QMath.h"
#include "Constants.h"

// Default constructor for EnemyShip
EnemyShip::EnemyShip()
{
}

// Parameterized constructor for EnemyShip
EnemyShip::EnemyShip(Vec3 pos, Model* model_, int health_)
{

    // Initialize the transform with position, orientation, and scale
    transform = Transform(pos, Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(3.0f, 3.0f, 3.0f));
    // Create a new Body object and associate it with the transform
    body = new Body(&transform, Vec3(), Vec3(), 1);

    // Initialize the model pointer
    model = model_;
    // Initialize the health
    health = health_;
  
    damage = 5;

}

// Set the target position for the enemy ship
void EnemyShip::setTarget(Vec3 target_)
{
    target = target_;
}

// Get the direction vector from the ship to the target
Vec3 EnemyShip::getTargetDirection()
{
    return VMath::normalize(target - transform.getPos());
}

// Called when the enemy ship is created
bool EnemyShip::OnCreate()
{
    // Create a collision sphere with a radius of 5.0 units
    collisionSphere = new Sphere(transform.getPos(), 5.0f);

    // Play looping rocket sound at the ship's position
    audioManager->PlaySound3DLooped("Rocket_Loop", transform.getPos());

    // Print creation message
    printf("Ship Created! \n");

    // Set the initial speed
    speed = 0.1;

    return true;
}

// Called when the enemy ship is destroyed
void EnemyShip::OnDestroy()
{
    // Set the model pointer to null
    model = nullptr;

    // Destroy the Body object and free its memory
    body->OnDestroy();
    delete body;

    // Destroy the exhaust trail
    exhaustTrail.OnDestroy();
}

// Set the index for the enemy ship
void EnemyShip::setIndex(int index)
{
    shipIndex = index;
}

// Update the enemy ship's state
void EnemyShip::Update(const float deltaTime)
{
    // Apply force to the body in the direction of the target, scaled by speed
    body->ApplyForce(getTargetDirection() * speed);
    // Update the body's state with the time delta
    body->Update(deltaTime);
    // Set the ship's orientation to face the target direction
    transform.setOrientation(QMath::lookAt(getTargetDirection(), UP));
    // Update the collision sphere's center to the ship's position
    collisionSphere->center = transform.getPos();

    // Update the exhaust trail's model matrix
    exhaustTrail.modelMat = transform.toModelMatrix();
}

// Render the enemy ship using the specified shader
void EnemyShip::Render(Shader* shader) const
{
    if (model != nullptr) {
        // Bind the model's textures
        model->BindTextures(0, 0);
        // Set the model matrix uniform in the shader
        glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
        // Render the model's mesh with triangles
        model->mesh->Render(GL_TRIANGLES);
        // Unbind the model's textures
        model->UnbindTextures();
    }
}

// Handle the enemy ship being hit
void EnemyShip::Hit()
{
    // Reduce the ship's health by 1
    health -= 1;
    // Play a sound indicating the ship was hit
    audioManager->PlaySound3D("Enemy_Hit", transform.getPos());

    // If health drops below 0, mark the ship for deletion
    if (health < 0) {
        deleteMe = true;
        // Play explosion sound at the ship's position
        audioManager->PlaySound3D("Enemy_Explosion", transform.getPos());
    }
}
