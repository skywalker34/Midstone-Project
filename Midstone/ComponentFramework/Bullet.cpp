#include "Bullet.h"

Bullet::Bullet(Transform spawnPos, float speed_, Vec3 forward_){
	transform = spawnPos;
	speed = speed_;
	forward = forward_;
}

bool Bullet::OnCreate()
{
	model = Model("Sphere.obj");
	if (model.OnCreate() == false) return false;
	transform.setScale(0.2f, 0.2f, 0.2f);

	collisionSphere = new Sphere(transform.getPos(), 1.0f); //set the collision sphere

	return true;
}

void Bullet::OnDestroy()
{
	
	model.OnDestroy();
}

void Bullet::Update(const float deltaTime)
{
	collisionSphere->center = transform.getPos(); //ensure the sphere's position remains the same as the bullet 

	transform.setPos(transform.getPos() + (forward * speed));
	if (abs(VMath::mag(transform.getPos() - initialPos) > maxRange)) {
		deleteMe = true;
	}

}

void Bullet::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, color);
	model.mesh->Render(GL_TRIANGLES);
}
