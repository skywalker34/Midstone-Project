#include "FriendlyShip.h"


FriendlyShip::FriendlyShip()
{
	transform = Transform(Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(0.02f, 0.02f, 0.02f));
	body = new Body(transform, Vec3(), Vec3(), 1);

	
	
	printf("FriendlyShip Constructor: Transform initialized with position (%f, %f, %f)\n", transform.getPos().x, transform.getPos().y, transform.getPos().z);
	
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
		transform = body->Update(deltaTime, transform);
		//keeps the ship pointing toward where its going
		Vec3 axis = VMath::cross(Vec3(0, 0, -1), moveDirection); //use the foward vector (negative z and diretion to get the axis of rotation)
		float targetAngle = acos(VMath::dot(Vec3(0, 0, -1), moveDirection)) * RADIANS_TO_DEGREES;

		newAngle = targetAngle > newAngle ? newAngle + 1 : targetAngle;	// Not done yet
		
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
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	glUniform4fv(shader->GetUniformID("meshColor"), 1, color);
	model.mesh->Render(GL_TRIANGLES);
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