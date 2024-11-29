#include "Planet.h"
#include "QMath.h"
#include "Constants.h"

// Default constructor for Planet
Planet::Planet()
{
}

// Parameterized constructor for Planet
Planet::Planet(float radius_, int health_, Model* model_, Vec3 pos) {
    radius = radius_; // Initialize the radius
    health = health_; // Initialize the health

    model = model_; // Set the model

    // Set the position and scale of the planet
    transform.setPos(pos);
    transform.setScale(radius, radius, radius);
    transform.setOrientation(QMath::angleAxisRotation(180, Vec3(0, 0, 1)));

    // Initialize the atmosphere and set its scale
    atmosphere = transform;
    atmosphere.setScale(radius + 1, radius + 1, radius + 1);
}

// Called when the planet is created
bool Planet::OnCreate()
{
    // Create a collision sphere with the planet's radius
    collisionSphere = new Sphere(transform.getPos(), radius);
    return true;
}

// Called when the planet is destroyed
void Planet::OnDestroy()
{
    // Delete the collision sphere
    delete collisionSphere;
}

// Update the planet's state
void Planet::Update(float deltaTime)
{
    // Rotate the atmosphere around the Y-axis
    Quaternion q = QMath::angleAxisRotation(deltaTime, Vec3(0, 1, 0));
    atmosphere.setOrientation(atmosphere.getOrientation() * q);

    // Rotate the planet around the Y-axis in the opposite direction
    q = QMath::angleAxisRotation(deltaTime / 2, Vec3(0, -1, 0));
    transform.setOrientation(transform.getOrientation() * q);
}

// Render the planet using the specified shader
void Planet::Render(Shader* shader) const
{
    // Bind the model's textures and set the model matrix in the shader
    model->BindTextures(0, 1);
    glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
    model->mesh->Render(GL_TRIANGLES);
    model->UnbindTextures();

    // Enable blending and render the atmosphere
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    model->BindTextures(2, 2);
    glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, atmosphere.toModelMatrix());
    model->mesh->Render(GL_TRIANGLES);
    model->UnbindTextures();
    glDisable(GL_BLEND);
}

// Handle the planet taking damage
void Planet::takeDamage(int damage)
{
    health -= damage; // Reduce health by the damage amount
    std::cout << "Health: " << health << std::endl; // Output the current health
}
