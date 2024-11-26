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

	
	Vec3 initialVelocities[10000];
	for (int i = 0; i < BUF_SIZE; i++) {
		initialVelocities[i] = Vec3(0.0, 0.0, 0.0);
	}

	//store all those velocities in a buffere
	glGenBuffers(1, &velBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * BUF_SIZE, initialVelocities, GL_DYNAMIC_COPY);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer); //bind the buffer
	glUseProgram(loadVerts->GetProgram());

	mesh->Render(GL_POINTS); //type of render doesn't really matter as there is nothing to see here
	glUseProgram(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); //unbind the buffer


	modelMat = MMath::translate(0, 5, -20);

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
}

void Explosion::Update(float deltaTime)
{
	if (!animComplete) {
		activeSimTime += deltaTime;
		for (int i = 0; i < debrisChunkCount; i++) {
			body[i]->Update(deltaTime);
			body[i]->UpdateOrientation(deltaTime);
		}
	}

	if (activeSimTime >= simTime) {
		animComplete = true;
		activeSimTime = 0;
	}
}

void Explosion::OnExplode()
{
	animComplete = false;
	for (int i = 0; i < debrisChunkCount; i++) {

		Vec3 rv1 = Vec3(fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1);
		Vec3 rv2 = Vec3(fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1);

		Vec3 rv = VMath::cross(rv1, rv2);

		body[i]->ApplyForce(rv); //create a random vector
		body[i]->angularVel = (Vec3(fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1, fmod(rand(), 2 * 2) - 1)); //create a random vector
		
	}
	onExplode = false;
}

void Explosion::ResetExplosion(ComputeShader* comp)
{
	for (int i = 0; i < debrisChunkCount; i++) {
		debrisTransforms[i]->setPos(Vec3());

		body[i]->vel = Vec3();
		body[i]->angularVel = Vec3();
	}

	glUseProgram(comp->GetProgram());

	glUniform1i(comp->GetUniformID("yDispatch"), 100); //amount of dispatches in the y direction(?) so the GPUs can work in parralel doing these calculations
	glDispatchCompute(100, 100, 1);//make sure the dispatch in the y parameter heres matches that in the uniform above
	glMemoryBarrier(GL_ALL_BARRIER_BITS);


	//unbind the buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	OnExplode();

}

void Explosion::RenderDebris(Shader* shader)
{
	if (!animComplete) {
		for (int i = 0; i < debrisChunkCount; i++) {


			glUseProgram(shader->GetProgram());
			glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, cam->GetViewMatrix());
			glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, cam->GetProjectionMatrix());
			glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, debrisTransforms[i]->toModelMatrix());
			glUniform4fv(shader->GetUniformID("meshColor"), 1, ORANGE);
			model->mesh->Render(GL_TRIANGLES);

		}
	}
}

void Explosion::Render(Shader* shader, ComputeShader* comp)
{

	if (!animComplete) {

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuffer);

		glUseProgram(comp->GetProgram());

		glUniform1i(comp->GetUniformID("yDispatch"), YDISPATCH); //amount of dispatches in the y direction(?) so the GPUs can work in parralel doing these calculations
		glUniform1f(comp->GetUniformID("simSpeed"), 60); //frequency 
		glUniform1f(comp->GetUniformID("randSeed"), rand()); //seed for rng
		glDispatchCompute(BUF_SIZE / YDISPATCH, YDISPATCH, 1);//make sure the dispatch in the y parameter heres matches that in the uniform above
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

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
	modelMat = MMath::translate(pos_);
	for (int i = 0; i < debrisChunkCount; i++) {
		debrisTransforms[i]->setParent(modelMat);
	}
}
