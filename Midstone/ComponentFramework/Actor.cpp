#include "Actor.h"
#include "Constants.h"

// Default constructor for Actor
Actor::Actor()
{
}

// Called when the actor is created
bool Actor::OnCreate()
{
    // Initialization logic can be added here
    return true;
}

// Called when the actor is destroyed
void Actor::OnDestroy()
{
    // Set the model pointer to nullptr to free up resources
    model = nullptr;
}

// Parameterized constructor for Actor
Actor::Actor(Transform transform_, Model* model_)
{
    // Initialize the transform and model
    transform = transform_;
    model = model_;
}

// Render the actor using the specified shader
void Actor::Render(Shader* shader) const
{
    // Set the model matrix uniform in the shader
    glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
    // Set the mesh color uniform in the shader
    glUniform4fv(shader->GetUniformID("meshColor"), 1, meshColour);
    // Render the model's mesh with triangles
    model->mesh->Render(GL_TRIANGLES);
}

// Destructor for Actor
Actor::~Actor() {}
