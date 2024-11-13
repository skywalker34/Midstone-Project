#include "Actor.h"
#include "Constants.h"

Actor::Actor()
{
}

bool Actor::OnCreate()
{
	
	return true;
}

void Actor::OnDestroy()
{
	model = nullptr;
}

Actor::Actor(Transform transform_, Model* model_)
{
	transform = transform_;
	model = model_;
}

void Actor::Render(Shader* shader) const
{

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, GREEN);
	model->mesh->Render(GL_TRIANGLES);
}



Actor::~Actor() {}
