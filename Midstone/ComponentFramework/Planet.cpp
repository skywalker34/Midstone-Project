#include "Planet.h"
#include "QMath.h"
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
	transform.setOrientation(QMath::angleAxisRotation(180, Vec3(0, 0, 1)));

	atmosphere = transform;
	atmosphere.setScale(radius + 1, radius + 1, radius + 1);

	
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
	//rotate the atmosphere
	Quaternion q = QMath::angleAxisRotation(deltaTime, Vec3(0, 1, 0));
	atmosphere.setOrientation(atmosphere.getOrientation() * q);
	q = QMath::angleAxisRotation(deltaTime / 2, Vec3(0, -1, 0));
	transform.setOrientation(transform.getOrientation() * q );

}

void Planet::Render(Shader* shader) const
{
	model->BindTextures(0, 1);//honestly don't know how to not hardcode this
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	model->mesh->Render(GL_TRIANGLES);
	model->UnbindTextures();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model->BindTextures(2, 2);//honestly don't know how to not hardcode this
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, atmosphere.toModelMatrix());
	model->mesh->Render(GL_TRIANGLES);
	model->UnbindTextures();
	glDisable(GL_BLEND);

}

void Planet::takeDamage(int damage)
{
	health -= damage; 
	std::cout << "Health: " << health << std::endl;
}
