#include "Bullet.h"
#include "QMath.h"
#include "constants.h"

Bullet::Bullet(Transform spawnPos, float speed_, Vec3 forward_, Model** model_){

	bulletModel = model_;
	transform = spawnPos;
	
	speed = speed_;
	forward = forward_;
}

bool Bullet::OnCreate()
{
	//create the bullet
	transform.setScale(0.5f, 0.5f, 0.5f);

	initialPos = transform.getPos();

	collisionSphere = new Sphere(transform.getPos(), 1.0f); //set the collision sphere

	//rotate the bullet so it faces the direction it was shot
	if (VMath::mag(forward) > VERY_SMALL) {
		Quaternion q = QMath::lookAt(forward, Vec3(0, 1, 0));
		transform.setOrientation(q);
	}

	return true;
}

void Bullet::OnDestroy()
{
	bulletModel = nullptr; //prevent danglers
}

void Bullet::Update(const float deltaTime)
{
	collisionSphere->center = transform.getPos(); //ensure the sphere's position remains the same as the bullet 

	transform.setPos(transform.getPos() + (forward * speed)); //move the bullet forward (no need for acceleration or anything so no physics body)


	

	//if the bullet goes too far, delete it 
	if (transform.getPos() != initialPos) {
		if (abs(VMath::mag(transform.getPos() - initialPos) > maxRange)) {
			deleteMe = true;
		}
	}

}

void Bullet::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	(*bulletModel)->mesh->Render(GL_TRIANGLES);
}
