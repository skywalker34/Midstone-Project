#include "Actor.h"

Actor::Actor()
{
}

bool Actor::OnCreate()
{
	if (model.OnCreate() == false) return false;
	return true;
}

Actor::Actor(Transform transform_, Model model_)
{
	transform = transform_;
	model = model_;
}

void Actor::Render(Shader* shader) const
{

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	model.mesh->Render(GL_TRIANGLES);
}



Actor::~Actor() {}
