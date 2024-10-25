#include "Bullet.h"
#include "QMath.h"
#include "constants.h"

Bullet::Bullet(Transform spawnPos, float speed_, Vec3 forward_, Model** model_){

	modelTemp = model_;
	transform = spawnPos;
	speed = speed_;
	forward = forward_;
}

bool Bullet::OnCreate()
{
	/*modelTemp = Model("Sphere.obj");
	if (modelTemp.OnCreate() == false) return false;*/
	transform.setScale(0.8f, 0.8f, 0.8f);

	collisionSphere = new Sphere(transform.getPos(), 1.0f); //set the collision sphere

	return true;
}

void Bullet::OnDestroy()
{
	
	
}

void Bullet::Update(const float deltaTime)
{
	collisionSphere->center = transform.getPos(); //ensure the sphere's position remains the same as the bullet 

	transform.setPos(transform.getPos() + (forward * speed));


	if (VMath::mag(forward) > VERY_SMALL) {
		Quaternion q = QMath::lookAt(forward, Vec3(0,1, 0));
		transform.setOrientation(q);
	}

	if (abs(VMath::mag(transform.getPos() - initialPos) > maxRange)) {
		deleteMe = true;
	}

}

void Bullet::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	//glUniform4fv(shader->GetUniformID("meshColor"), 1, color);
	(*modelTemp)->mesh->Render(GL_TRIANGLES);
}
