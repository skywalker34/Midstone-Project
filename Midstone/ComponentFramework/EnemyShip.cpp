#include "EnemyShip.h"



EnemyShip::EnemyShip()
{
}

EnemyShip::EnemyShip(Vec3 pos)
{
	transform = Transform(pos, Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(0.02f, 0.02f, 0.02f));
	body = new Body(&transform, Vec3(), Vec3(), 1);
	
}

void EnemyShip::setTarget(Vec3 target_)
{
	target = target_;

}

Vec3 EnemyShip::getTargetDirection()
{
	return VMath::normalize(target - transform.getPos());
}



bool EnemyShip::OnCreate()
{
	health = 5; //may want to put this in constructor

	model = Model("enemyShip.obj");
	collisionSphere = new Sphere(transform.getPos(), 1.0f);

	if (model.OnCreate() == false) return false;
	printf("Ship Created! \n");

	

	return true;
}

void EnemyShip::OnDestroy()
{
	model.OnDestroy();


	body->OnDestroy();
	delete body;

}

void EnemyShip::Update(const float deltaTime)
{
	body->ApplyForce(getTargetDirection() * speed);	//shouldn't have to do this every frame we nay want to move it
	body->Update(deltaTime);

	collisionSphere->center = transform.getPos();
}

void EnemyShip::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, Vec4((health / 5.0f) + 0.1f, 0.0f, 0.0f, 0.0f)); //for now just make the color correlate directly with health (5 is a temp hardcoded max health value)
	model.mesh->Render(GL_TRIANGLES);
}

void EnemyShip::Hit()
{
	health -= 1; //reduce health
	if (health < 0) { //if dead set flag so scene knows to delete the ship
		deleteMe = true;
	}

}
