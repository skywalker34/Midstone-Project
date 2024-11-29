#include "ExhaustTrail.h"
#include <glew.h>
#include <iostream>

#include <MMath.h>
#include "Debug.h"
#include "Constants.h"
#include "Camera.h"
#include "Mesh.h"

#include "ComputeShader.h"

bool ExhaustTrail::OnCreate(Camera* cam_, Shader* loadVerts, Mesh* mesh_)
{


	mesh = mesh_;
	cam = cam_;

	//create the buffer to hold the vertex positions between the vert stage and compute "stage"
	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * 10000, NULL, GL_DYNAMIC_COPY); //null canse the loadVertsToBuffer shader will handle it
	//the size of these buffers are bigger then they need to be because unless I deduplicate the mesh I can't get the true vertex count
	//thanks, artists

	//safest bet for the vertcount is multiply the listed size by 4 and then round up and you should have more than enough space
	//inefficient on storage/VRAM I know but I don't have any ideas on how to solve this problem

	//give all of our particles a starting velocity of 0 at the origin
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


	return true;
}

void ExhaustTrail::OnDestroy()
{
	mesh = nullptr;
	glDeleteBuffers(1, &posBuffer);
	glDeleteBuffers(1, &velBuffer);
}

void ExhaustTrail::Render(Shader* shader, ComputeShader* comp)
{


	if (mesh == nullptr) { //error check 

		printf("NO MESH!!!");
		return;

	}
	
	//bind the buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuffer);

	glUseProgram(comp->GetProgram());

	glUniform1i(comp->GetUniformID("yDispatch"), YDISPATCH); //amount of dispatches in the y direction(?) so the GPUs can work in parralel doing these calculations
	glUniform1f(comp->GetUniformID("simSpeed"), 60); //frequency of teh sim (delta time) fps
	glUniform1f(comp->GetUniformID("randSeed"), rand()); //seed for random generation done on the gpu
	glUniform3fv(comp->GetUniformID("forwardVector"), 1, Vec3(0, 0, -1)); //direction the "ship" is headed
	glDispatchCompute(100, YDISPATCH, 1);//make sure the dispatch in the y parameter heres matches that in the uniform above
	glMemoryBarrier(GL_ALL_BARRIER_BITS); //stop the rest of the shaders being executed until computing is done


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
