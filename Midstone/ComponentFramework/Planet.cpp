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
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, GREY);
	model->mesh->Render(GL_TRIANGLES);
}

void Planet::takeDame(int damage)
{
	health -= damage; 
}
