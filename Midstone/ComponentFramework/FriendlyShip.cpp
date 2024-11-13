#include "FriendlyShip.h"
#include "Sphere.h"



//depreciated
FriendlyShip::FriendlyShip()
{
	transform = Transform(Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f)), Vec3(1.0f, 1.0f, 1.0f));
	body = new Body(&transform, Vec3(), Vec3(), 1);

	
	
	//printf("FriendlyShip Constructor: Transform initialized with position (%f, %f, %f)\n", transform.getPos().x, transform.getPos().y, transform.getPos().z);
	
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
	
	printf("Ship Created! \n");
	
	rangeSphere = Model("Sphere.obj");

	if (rangeSphere.OnCreate() == false) return false;
	printf("Ship Created! \n");
	rangeSphereT = transform;
	rangeSphereT.setScale(Vec3(range, range, range)); //sphere mesh has radius of 2 units wo when we scale it by range we have to divide to get the actual range
	//we may want to go into 3dsmax and make a unit sphere 


	
	collisionSphere = new Sphere(transform.getPos(), 5.0f);
	speed = 5;
	return true;
}

void FriendlyShip::OnDestroy()
{

	model = nullptr;

	rangeSphere.OnDestroy();

	exhaustTrail.OnDestroy();

	delete collisionSphere;
	
	for (Bullet* bullet : bullets) {
		bullet->OnDestroy();
		delete bullet;
	}

	delete model;

}

void FriendlyShip::Update(const float deltaTime)
{
	
	if (!canFire) {
		timeSinceShot += deltaTime;
		canFire = timeSinceShot >= rateOfFire && !isMoving;
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update(deltaTime);
		if (bullets[i]->deleteMe) {
			bullets[i]->OnDestroy();
			delete bullets[i];
			bullets[i] = nullptr;
			bullets.erase(std::remove(bullets.begin(), bullets.end(), nullptr), bullets.end());
		}
	}

	if (hasReachDestination()) {
		isMoving = false;

		bool HappenOnce = true;
		if (HappenOnce == true && isMoving == false)
		{
			SoundEngineFlying->stopAllSounds();
			HappenOnce = false;
		}
	}

	if (isMoving) {
		slerpT = slerpT >= 1 ? 1 : slerpT + deltaTime;
		body->Update(deltaTime);
		rotateTowardTarget(movingDirection);
		isMoving = VMath::mag(destination - transform.getPos()) > 0.01;
	}
	else {
		body->vel = Vec3();
	}
	detectionSphere.center = transform.getPos();//update teh collision sphere to match the ships position
	collisionSphere->center = transform.getPos();


	if (displayRange) {
		rangeSphereT.setPos(detectionSphere.center);
	}


	exhaustTrail.modelMat = transform.toModelMatrix();

}

void FriendlyShip::Render(Shader* shader) const
{

	model->BindTextures(0,0);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, transform.toModelMatrix());
	
	model->mesh->Render(GL_TRIANGLES);

	if (displayRange) {
		
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, rangeSphereT.toModelMatrix());
		//glUniform4fv(shader->GetUniformID("meshColor"), 1, color);
		rangeSphere.mesh->Render(GL_LINES);
	}

	model->UnbindTextures();
}

void FriendlyShip::RenderBullets(Shader* shader) const
{
	for (Bullet* bullet : bullets) {
		bullet->Render(shader);
	}
}

void FriendlyShip::FindClosestEnemy(EnemyShip* enemy)
{	
	if (!isSwitchingTarget) {
		currentTargetDistance = VMath::mag(transform.getPos() - closestEnemy->transform.getPos());
		potentialTargetDistance = VMath::mag(transform.getPos() - enemy->transform.getPos());
		potentialTarget = enemy;
		potentialTarget->transform.getPos().print("potential target");
	}
	
	if (potentialTargetDistance < currentTargetDistance) {
		isSwitchingTarget = true;
		rotateTowardTarget(potentialTarget->aimingPoint);
	}
}

void FriendlyShip::Fire()
{
    canFire = false; 
    timeSinceShot = 0;

    // Calculate the direction the ship is facing
    

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
	irrklang::vec3df position(bulletSpawn.x, bulletSpawn.y, bulletSpawn.z);
	int RandomChance = rand() % 3 + 1; // Random number From 1 to 3
	if (RandomChance == 1)
	{
		SoundEngine->play3D("audio/LaserShooting.mp3", position, false); // Audio For Shooting Noise
		SoundEngine->setSoundVolume(1.0f);
	}
	if (RandomChance == 2)
	{
		SoundEngine->play3D("audio/DisturbedShooting.mp3", position, false); // Audio For Shooting Noise
		SoundEngine->setSoundVolume(1.0f);

	}
	if (RandomChance == 3)
	{
		SoundEngine->play3D("audio/PewShoot.mp3", position, false); // Audio For Shooting Noise
		SoundEngine->setSoundVolume(1.0f);
	}
}


void FriendlyShip::moveToDestination(Vec3 destination_)
{
	destination = destination_;
	isMoving = true;
	slerpT = 0;

	
	irrklang::vec3df BodyPosition(body->pos.x, body->pos.y, body->pos.z);  
	SoundEngineFlying->setSoundVolume(0.3f);
	SoundEngineFlying->play3D("audio/RocketFlying.mp3", BodyPosition, true); // Broken Atm causes crash?
	

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
	return VMath::mag(transform.getPos() - destination) < 1.0f;
}


FriendlyShip::~FriendlyShip() {}