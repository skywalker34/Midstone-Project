#include "Planet.h"
#include "Constants.h"



Planet::Planet()
{
}

Planet::Planet(float radius_, int health_, Model* model_, Vec3 pos) {
	radius = radius_;
	health = health_;

	model = model_;

	transform.setPos(pos);
	transform.setScale(radius, radius, radius);

	
}


bool Planet::OnCreate()
{

	collisionSphere = new Sphere(transform.getPos(), radius);
	return true;
}

void Planet::OnDestroy()
{
	delete collisionSphere;
}

void Planet::Update(float deltaTime)
{
}

void Planet::Render(Shader* shader) const
{
	model->BindTextures(0, 1);//honestly don't know how to not hardcode this

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	model->mesh->Render(GL_TRIANGLES);

	model->UnbindTextures();
}

void Planet::takeDame(int damage)
{
	health -= damage; 
}
