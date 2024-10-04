#include "EnemyShip.h"



EnemyShip::EnemyShip()
{
}

EnemyShip::EnemyShip(Vec3 pos)
{
	transform = Transform(pos, Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(0.02f, 0.02f, 0.02f));
	body = new Body(transform, Vec3(), Vec3(), 1);
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
	model = Model("enemyShip.obj");
	

	if (model.OnCreate() == false) return false;
	printf("Ship Created! \n");

	setTarget(Vec3(0, 0, 0));	///temporary remove later

	return true;
}

void EnemyShip::OnDestroy()
{
	model.OnDestroy();
}

void EnemyShip::Update(const float deltaTime)
{
	body->ApplyForce(getTargetDirection() * speed);	//shouldn't have to do this every frame we nay want to move it
	transform = body->Update(deltaTime, transform);
}

void EnemyShip::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, Vec4(1.0f, 0.0f, 0.0f, 0.0f));
	model.mesh->Render(GL_TRIANGLES);
}
