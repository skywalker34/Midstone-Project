#include "FriendlyShip.h"


FriendlyShip::FriendlyShip()
{
	transform = Transform(Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(0.02f, 0.02f, 0.02f));
	body = new Body(transform, Vec3(), Vec3(), 1);

	
	
	//printf("FriendlyShip Constructor: Transform initialized with position (%f, %f, %f)\n", transform.getPos().x, transform.getPos().y, transform.getPos().z);
	
}

bool FriendlyShip::OnCreate()
{
	model = Model("Ship.obj");
	if (model.OnCreate() == false) return false;
	printf("Ship Created! \n");
	
	Fire();
	return true;
}

void FriendlyShip::OnDestroy()
{
	
	model.OnDestroy();
	for (Bullet* bullet : bullets) {
		bullet->OnDestroy();
		delete bullet;
	}
}

void FriendlyShip::Update(const float deltaTime)
{

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update(deltaTime);
		if (bullets[i]->deleteMe) {
			bullets[i]->OnDestroy();
			delete bullets[i];
			bullets[i] = nullptr;
			bullets.erase(std::remove(bullets.begin(), bullets.end(), nullptr), bullets.end());

		}
	}

	if (isMoving) {
		transform = body->Update(deltaTime, transform);
		//keeps the ship pointing toward where its going
		//Vec3 axis = VMath::cross(Vec3(0, 0, -1), moveDirection); //use the foward vector (negative z and diretion to get the axis of rotation)
		Vec3 axis = Vec3(0, 0, -1);
		float targetAngle = acos(VMath::dot(Vec3(transform.getPos().x, transform.getPos().y, 0), moveDirection)) * RADIANS_TO_DEGREES;
		newAngle = targetAngle > newAngle ? newAngle + 1 : targetAngle;	// Not done yet

		if (targetAngle > newAngle) {

		}
		std::cout << targetAngle << std::endl;
		Quaternion newTransform = QMath::angleAxisRotation(newAngle, axis);	// Not done yet
	
		transform.setOrientation(newTransform);
		isMoving = VMath::mag(destination - transform.getPos()) > 0.01;
	}
	else {
		body->vel = Vec3();
	}
	
}

void FriendlyShip::Render(Shader* shader) const
{
	for (Bullet* bullet : bullets) {
		bullet->Render(shader);
	}

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, color);
	model.mesh->Render(GL_TRIANGLES);
}




void FriendlyShip::Fire()
{

	//the third parameter here "BACKWARD" should instead be the direction the ship is facing
	bullets.push_back(new Bullet(transform, 0.1f, BACKWARD));
	if (bullets.back()->OnCreate() == false) {
		printf("Bullet failed! /n");
	}
}

void FriendlyShip::moveToDestination(Vec3 destination_)
{
	destination = destination_;
	isMoving = true;
	newAngle = 0;
	if (wouldIntersectPlanet) {
		
		Vec3 axis = VMath::cross(destination, transform.getPos());
		Quaternion newPosition = QMath::angleAxisRotation(1.0f, axis);
		moveDirection = QMath::rotate(transform.getPos(), newPosition) - transform.getPos();
		body->vel = speed * VMath::normalize(moveDirection);
	}
	else {
		Vec3 diff =  destination - transform.getPos(); //"draw" a vector between the 2 points
		moveDirection = VMath::normalize(diff);//"convert" thevector into just a direction (normalize)
		body->vel = moveDirection * speed; //tell the ship to move along that vector
	}
}

bool FriendlyShip::hasReachDestination()
{
	return VMath::mag(body->pos - destination) < 0.1f;
}

FriendlyShip::~FriendlyShip() {}