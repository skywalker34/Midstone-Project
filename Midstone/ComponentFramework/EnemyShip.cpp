#include "EnemyShip.h"
#include"QMath.h"
#include"Constants.h"


EnemyShip::EnemyShip()
{
}

EnemyShip::EnemyShip(Vec3 pos, Model* model_, int health_)
{
	transform = Transform(pos, Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(3.0f, 3.0f, 3.0f));
	body = new Body(&transform, Vec3(), Vec3(), 1);
	
	model = model_;
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
	

	collisionSphere = new Sphere(transform.getPos(), 5.0f);



	audioManager->PlaySound3DLooped("Rocket_Loop", transform.getPos());

	printf("Ship Created! \n");

	speed = 0.1;
	

	return true;
}

void EnemyShip::OnDestroy()
{
	model = nullptr;

	

	body->OnDestroy();
	delete body;

	exhaustTrail.OnDestroy();



}

void EnemyShip::setIndex(int index)
{
	shipIndex = index;
}

void EnemyShip::Update(const float deltaTime)
{
	irrklang::vec3df bodypos(body->pos.x, body->pos.y, body->pos.z);
	bool happenonce = true;
	

	body->ApplyForce(getTargetDirection() * speed);	//shouldn't have to do this every frame we nay want to move it
	body->Update(deltaTime);
	transform.setOrientation(QMath::lookAt(getTargetDirection(), UP));
	collisionSphere->center = transform.getPos();

	exhaustTrail.modelMat = transform.toModelMatrix();
}

void EnemyShip::Render(Shader* shader) const
{
	if (model != nullptr) {
		model->BindTextures(0,0);
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
		model->mesh->Render(GL_TRIANGLES);
		model->UnbindTextures();

	}
}

void EnemyShip::Hit()
{
	health -= 1; //reduce health
	audioManager->PlaySound3D("Enemy_Hit", transform.getPos());

	if (health < 0) { //if dead set flag so scene knows to delete the ship
		deleteMe = true;
		audioManager->PlaySound3D("Enemy_Explosion", transform.getPos());
	}

}
