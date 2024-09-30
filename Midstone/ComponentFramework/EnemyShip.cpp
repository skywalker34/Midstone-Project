#include "EnemyShip.h"

bool EnemyShip::OnCreate()
{
	model = Model("enemyShip.obj");
	transform.setScale(Vec3(0.02, 0.02, 0.02));

	if (model.OnCreate() == false) return false;
	printf("Ship Created! \n");

	return true;
}

void EnemyShip::OnDestroy()
{
	model.OnDestroy();
}

void EnemyShip::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, Vec4(1.0f, 0.0f, 0.0f, 0.0f));
	model.mesh->Render(GL_TRIANGLES);
}
