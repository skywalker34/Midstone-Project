#include "Explosion.h"


bool Explosion::OnCreate(Camera* cam_, Shader* loadVerts, Mesh* mesh_, Model* model_)
{
	mesh = mesh_;
	cam = cam_;

	//create the buffer to hold the vertex positions between the vert stage and compute "stage"
	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * BUF_SIZE, NULL, GL_DYNAMIC_COPY); //null canse the loadVertsToBuffer shader will handle it
	//the size of these buffers are bigger then they need to be because unless I deduplicate the mesh I can't get the true vertex count
	//thanks, artists

	//safest bet for the vertcount is multiply the listed size by 4 and then round up and you should have more than enough space
	//inefficient on storage/VRAM I know but I don't have any ideas on how to solve this problem

	//3 floats per vec = 12 bytes, 10,000 vecs = 120Kb of VRAM for each particle shader in the scene (I think)

	//ensure all the particles start with a 0 vel
	Vec3 initialVelocities[10000];
	for (int i = 0; i < BUF_SIZE; i++) {
		initialVelocities[i] = Vec3(0.0, 0.0, 0.0);
	}

	//store all those velocities in a buffere
	glGenBuffers(1, &velBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * BUF_SIZE, initialVelocities, GL_DYNAMIC_COPY);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer); //bind the buffer
	glUseProgram(loadVerts->GetProgram()); //run a shader to isolate the vert pos from scott's mesh buffer

	mesh->Render(GL_POINTS); //type of render doesn't really matter as there is nothing to see here
	glUseProgram(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); //unbind the buffer


	
	//create transforms for the debris meshes (larer particles taht use a 3d mesh instea dof points)
	for (int i = 0; i < debrisChunkCount; i++) {
		debrisTransforms.push_back(new Transform());
		debrisTransforms[i]->setParent(modelMat);
		debrisTransforms[i]->setScale(Vec3(0.5, 0.5, 0.5));

		body.push_back(new Body(debrisTransforms[i], Vec3(), Vec3(), 1));
	}

	model = model_;

	

	return true;
}

void Explosion::OnDestroy()
{
	//everything is flyweight not much to delete
	//here for future improvent

	glDeleteBuffers(1, &posBuffer);
	glDeleteBuffers(1, &velBuffer);
}


void Explosion::Update(float deltaTime)
{
	if (!animComplete) { //if the explosion "animation" is running
		activeSimTime += deltaTime; //get teh amount of time the animation/simulation has been running
		for (int i = 0; i < debrisChunkCount; i++) { //update the physics body of all the debris
			//teh debris are all childed(?) to the explosion particle effect so physics calcs are done in local debris spac eif you will
			body[i]->Update(deltaTime);
			body[i]->UpdateOrientation(deltaTime);
		}
	}

	//if the simulation/animation has reached its max time, stop i
	if (activeSimTime >= simTime) {
		animComplete = true;
		activeSimTime = 0;
	}
}

void Explosion::OnExplode()
{
	animComplete = false;//tell the program the animation/simulation has begun
	for (int i = 0; i < debrisChunkCount; i++) { //for each of the larger debris meshes...

		//create 2 random vectors
		Vec3 rv1 = Vec3(fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1);
		Vec3 rv2 = Vec3(fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1);
		//then cross them. this is so there's more variation in the flightpaths of the debris.
		//at least it looked better then when I was just generating 1 random vec
		Vec3 rv = VMath::cross(rv1, rv2);

		//apply a force using the random vector (no need to normalize so that there's force too)
		body[i]->ApplyForce(rv); //create a random vector
		//also give the debris a random spin
		body[i]->angularVel = (Vec3(fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1)); //create a random vector
		
	}
	onExplode = false; //tell the program the inital explosion is done
}

void Explosion::ResetExplosion(ComputeShader* comp)
{
	//null out all physics values and reset position to local 0 for the debris chunks
	for (int i = 0; i < debrisChunkCount; i++) {
		debrisTransforms[i]->setPos(Vec3());

		body[i]->vel = Vec3();
		body[i]->angularVel = Vec3();
	}

	//run a compute shader to reset all the particle pos to 0

	glUseProgram(comp->GetProgram());
	 
	glUniform1i(comp->GetUniformID("yDispatch"), YDISPATCH); //amount of dispatches in the y direction(?) so the GPUs can work in parralel doing these calculations
	glDispatchCompute(100, YDISPATCH, 1);//make sure the dispatch in the y parameter heres matches that in the uniform above
	glMemoryBarrier(GL_ALL_BARRIER_BITS);


	//unbind the buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	
	OnExplode();

}

void Explosion::RenderDebris(Shader* shader)
{
	//render the debris particles with teh smae mesh
	if (!animComplete) { //if the animation is runnign
		//load the uniforms universal for each debris chunk
		glUseProgram(shader->GetProgram());
		glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, cam->GetViewMatrix());
		glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, cam->GetProjectionMatrix());

		for (int i = 0; i < debrisChunkCount; i++) {
			//for each debris chunk set its unique pos and render
			glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, debrisTransforms[i]->toModelMatrix());
			model->mesh->Render(GL_TRIANGLES);

		}
	}
}

void Explosion::Render(Shader* shader, ComputeShader* comp)
{

	if (!animComplete) { //if the anim is running

		//bind and run the compute
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuffer);

		glUseProgram(comp->GetProgram());

		glUniform1i(comp->GetUniformID("yDispatch"), YDISPATCH); //amount of dispatches in the y direction(?) so the GPUs can work in parralel doing these calculations
		glUniform1f(comp->GetUniformID("simSpeed"), 60); //frequency for the simulation (delta time)
		glUniform1f(comp->GetUniformID("randSeed"), rand()); //seed for rng
		glDispatchCompute(BUF_SIZE / YDISPATCH, YDISPATCH, 1);//make sure the dispatch in the y parameter heres matches that in the uniform above
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		//render the particle mesh with the compute shader changes applied
		glUseProgram(shader->GetProgram());
		glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, cam->GetProjectionMatrix());
		glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, cam->GetViewMatrix());
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMat);
		mesh->Render(GL_POINTS);

		//unbind the buffers
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

		glUseProgram(0);
	}
	
}

void Explosion::setPos(Vec3 pos_)
{
	//sets the model matrix of the explosion to the position and parents the debris to it
	modelMat = MMath::translate(pos_);
	for (int i = 0; i < debrisChunkCount; i++) {
		debrisTransforms[i]->setParent(modelMat);
	}
}
