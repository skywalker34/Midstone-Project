#include "FriendlyShip.h"
#include "Sphere.h"
#include "Collision.h"


//depreciated
FriendlyShip::FriendlyShip()
{
	transform = Transform(Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(1.0f, 1.0f, 1.0f));
	body = new Body(&transform, Vec3(), Vec3(), 1);


}

FriendlyShip::FriendlyShip(Model* model_, Model* bulletModel_)
{
	transform = Transform(Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(4.0f, 4.0f, 4.0f));
	body = new Body(&transform, Vec3(), Vec3(), 1);


	model = model_;
	bulletModel = bulletModel_;
}

bool FriendlyShip::OnCreate()
{

	detectionSphere = Sphere(transform.getPos(), range);





	rangeSphereT = transform;
	rangeSphereT.setScale(Vec3(range, range, range)); //sphere mesh has radius of 2 units wo when we scale it by range we have to divide to get the actual range
	//we may want to go into 3dsmax and make a unit sphere 



	collisionSphere = new Sphere(transform.getPos(), collisionSphereRadius);
	speed = 5;
	return true;
}

void FriendlyShip::OnDestroy()
{

	model = nullptr;



	exhaustTrail.OnDestroy();

	delete collisionSphere;

	for (Bullet* bullet : bullets) {
		bullet->OnDestroy();
		delete bullet;
	}





}

void FriendlyShip::Update(const float deltaTime)
{
	ResetFire(deltaTime);
	UpdateBullet(deltaTime);
	StopSound3DLooped();
	ShipMovement(deltaTime);

	if (displayRange) {
		rangeSphereT.setPos(detectionSphere.center);
	}

	exhaustTrail.modelMat = transform.toModelMatrix();
}

void FriendlyShip::ResetFire(const float deltaTime)
{
	if (!canFire) {
		timeSinceShot += deltaTime;
		canFire = timeSinceShot >= rateOfFire && !isMoving;
	}
}

void FriendlyShip::UpdateBullet(const float deltaTime)
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
}

void FriendlyShip::StopSound3DLooped()
{
	if (HappenOnce == true && isMoving == false)
	{
		audioManager->StopSound3DLooped(rocketSoundIndex);
		HappenOnce = false;
	}
}

void FriendlyShip::ShipMovement(const float deltaTime)
{
	if (wouldIntersectPlanet) {
		Orbit(orbitAxis);
		CheckIntersection(transform.getPos());
		MoveDirectly();
	}

	if (isMoving) {
		slerpT = slerpT >= 1 ? 1 : slerpT + deltaTime;
		body->Update(deltaTime);
		RotateTowardTarget(movingDirection);
		isMoving = !HasReachDestination();

	}
	else {
		body->vel = Vec3();
	}

	if (isChasing && !activeTarget->deleteMe) { //if player clicks on a ship then on an enemy the ship enters chasing mode
		ChasingEnemy();
	}

	detectionSphere.center = transform.getPos();	//update the collision sphere to match the ships position
	collisionSphere->center = transform.getPos();
}

void FriendlyShip::ChasingEnemy()
{
	destination = activeTarget->transform.getPos(); //set the destination to the enemy's pos
	MoveToDestination(destination);
	if (COLLISION::SphereSphereCollisionDetected(&detectionSphere, activeTarget->collisionSphere)) {
		//reset the variables
		isMoving = false;
		activeTarget = nullptr;
		isChasing = false;

	}
	else {
		isMoving = true;
	}
}

void FriendlyShip::Render(Shader* shader) const
{

	model->BindTextures(0, 0);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());

	model->mesh->Render(GL_TRIANGLES);

	model->UnbindTextures();




}

void FriendlyShip::RenderBullets(Shader* shader) const
{
	for (Bullet* bullet : bullets) {
		bullet->Render(shader);
	}
}

void FriendlyShip::RenderRange(Shader* shader) const
{
	if (displayRange) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, rangeSphereT.toModelMatrix());
		glUniform4fv(shader->GetUniformID("meshColor"), 1, Vec4(0.2, 0.3, 0.5, 0.4));
		rangeSphere->mesh->Render(GL_LINES);
		glDisable(GL_BLEND);
	}
}

void FriendlyShip::FindClosestEnemy(EnemyShip* enemy)
{
	if (!isSwitchingTarget) {
		currentTargetDistance = VMath::mag(transform.getPos() - closestEnemy->transform.getPos());
		potentialTargetDistance = VMath::mag(transform.getPos() - enemy->transform.getPos());
		potentialTarget = enemy;

		potentialTarget->aimingPoint = potentialTarget->transform.getPos() + potentialTarget->body->vel;

	}

	bool isInRange = COLLISION::SphereSphereCollisionDetected(&detectionSphere, enemy->collisionSphere);

	if (potentialTargetDistance < currentTargetDistance && isInRange) {
		isSwitchingTarget = true;
		RotateTowardTarget(potentialTarget->aimingPoint - transform.getPos());
	}
}

void FriendlyShip::Fire()
{
	canFire = false;
	timeSinceShot = 0;


	// Add some randomness to the direction to simulate flak guns
	float randomOffsetX = static_cast<float>(rand()) / RAND_MAX - 1.0f;
	float randomOffsetY = static_cast<float>(rand()) / RAND_MAX - 1.0f;
	float randomOffsetZ = static_cast<float>(rand()) / RAND_MAX - 1.0f;
	Vec3 randomOffset = VMath::normalize(Vec3(randomOffsetX, randomOffsetY, randomOffsetZ));

	// Adjust the target direction slightly
	Vec3 adjustedDirection = VMath::normalize(targetDirection + randomOffset * 0.1f); // Adjust the scale as needed

	bullets.push_back(new Bullet(transform, 1.0f, adjustedDirection, &bulletModel));
	if (bullets.back()->OnCreate() == false) {
		printf("Bullet failed! \n");
	}

	// Audio
	Vec3 bulletSpawn = transform.getPos();
	audioManager->PlaySound3D("Ship_Shooting", transform.getPos());

}


void FriendlyShip::MoveToDestination(Vec3 destination_)
{
	destination = destination_;
	isMoving = true;
	HappenOnce = true;

	CheckIntersection(transform.getPos());

	if (wouldIntersectPlanet) {
		orbitAxis = VMath::cross(destination, transform.getPos());
		slerpT = 0;
	}
	MoveDirectly();
}

void FriendlyShip::CheckIntersection(Vec3 initailPosition)
{
	float angle = acos(VMath::dot(VMath::normalize(initailPosition - ORIGIN), VMath::normalize(initailPosition - destination)));
	wouldIntersectPlanet = VMath::mag(initailPosition - ORIGIN) * sin(angle) < PLANET_RADIUS + collisionSphereRadius;
}

void FriendlyShip::Orbit(Vec3 axis)
{
	Quaternion newPosition = QMath::angleAxisRotation(1.0f, axis);
	movingDirection = QMath::rotate(transform.getPos(), newPosition) - transform.getPos();
	if (slerpT >= 1) {
		Quaternion rotationQuad = QMath::lookAt(movingDirection, UP);
		transform.setOrientation(rotationQuad);
		body->vel = speed * VMath::normalize(movingDirection);
	}
}

void FriendlyShip::MoveDirectly()
{
	if (!wouldIntersectPlanet) {
		slerpT = 0;
		Vec3 diff = destination - transform.getPos();	//"draw" a vector between the 2 points
		movingDirection = VMath::normalize(diff);		//"convert" thevector into just a direction (normalize)
		body->vel = movingDirection * speed;			//tell the ship to move along that vector
	}
}

void FriendlyShip::RotateTowardTarget(Vec3 targetDirection)
{
	if (VMath::mag(targetDirection) < 0.0001) return;
	//keeps the ship pointing toward where its going
	if (slerpT < 1) {

		if (VMath::mag(targetDirection) != 0) {
			Quaternion startQuad = QMath::lookAt(initialDirection, UP);
			Quaternion targetQuad = QMath::lookAt(targetDirection, UP);
			Quaternion currentQuat = QMath::normalize(QMath::slerp(startQuad, targetQuad, slerpT));

			transform.setOrientation(currentQuat);
		}

	}
	else {
		initialDirection = targetDirection;
	}
}

bool FriendlyShip::HasReachDestination()
{
	return VMath::mag(transform.getPos() - destination) < 1.0f;
}


FriendlyShip::~FriendlyShip() {}