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
	
	return true;
}

void FriendlyShip::OnDestroy()
{
	
	model.OnDestroy();
}

void FriendlyShip::Update(const float deltaTime)
{
	
	if (isMoving) {
		slerpT = slerpT >= 1 ? 1 : slerpT + deltaTime;
		transform = body->Update(deltaTime, transform);
		rotateTowardTarget(movingDirection);
		isMoving = VMath::mag(destination - transform.getPos()) > 0.01;
	}
	else {
		body->vel = Vec3();
	}
	
}

void FriendlyShip::Render(Shader* shader) const
{
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, color);
	model.mesh->Render(GL_TRIANGLES);
}

void FriendlyShip::moveToDestination(Vec3 destination_)
{
	destination = destination_;
	isMoving = true;
	slerpT = 0;
	if (wouldIntersectPlanet) {
		Vec3 axis = VMath::cross(destination, transform.getPos());
		Quaternion newPosition = QMath::angleAxisRotation(1.0f, axis);
		movingDirection = QMath::rotate(transform.getPos(), newPosition) - transform.getPos();
		body->vel = speed * VMath::normalize(movingDirection);
	}
	else {
		Vec3 diff =  destination - transform.getPos(); //"draw" a vector between the 2 points
		movingDirection = VMath::normalize(diff);//"convert" thevector into just a direction (normalize)
		body->vel = movingDirection * speed; //tell the ship to move along that vector
	}
}

void FriendlyShip::rotateTowardTarget(Vec3 target)
{
	//keeps the ship pointing toward where its going
	if (slerpT < 1) {
		Quaternion startQuad = QMath::lookAt(initialDirection, UP);
		Quaternion targetQuad = QMath::lookAt(target, UP);
		Quaternion currentQuat = QMath::slerp(startQuad, targetQuad, slerpT);
		transform.setOrientation(currentQuat);
	}
	else {
		initialDirection = target;
	}
}

bool FriendlyShip::hasReachDestination()
{
	return VMath::mag(body->pos - destination) < 0.1f;
}


FriendlyShip::~FriendlyShip() {}