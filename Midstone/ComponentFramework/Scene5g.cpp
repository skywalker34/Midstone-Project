#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene5g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Constants.h"
#include <QMath.h>
#include "Body.h"



Scene5g::Scene5g() {
	Debug::Info("Created Scene3p: ", __FILE__, __LINE__);
}

Scene5g::~Scene5g() {
	Debug::Info("Deleted Scene3p: ", __FILE__, __LINE__);
}

bool Scene5g::OnCreate() {
	Debug::Info("Loading assets Scene3p: ", __FILE__, __LINE__);

	computeShader = new ComputeShader("shaders/Explosion.glsl");	//create the compute shader
	if (computeShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}
	resetParticles = new ComputeShader("shaders/ResetParticles.glsl");	//create the compute shader
	if (resetParticles->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	//this shader isn't really a shader, I had this idea driving to work on 04/16/2024
	//yeah this is probably terrible practice however its the easiest way I could think of to get the actual vertex positions in correspondeace to their shader gl_vertexID
	//I COULD have tried to use the buffer Scott made in mesh.cpp however said buffer seems to have normals, UVs and vert pos and I'm not smart enough to get just the pos from it
	//one thing I should add is that it doesn't solve the fact that the OBJ file lies about the actual vertex pos (thank you vertex duplication) and count so I still need to declare my buffer to be much larger than it needs to be 
	loadVertsToBuffer = new Shader("shaders/loadVerticiesVert.glsl", "shaders/loadVerticiesFrag.glsl");
	if (loadVertsToBuffer->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	//Don't let the names fool you, this shader no longer has anything to do with textures (anymore)
	//I'm just too lazy to change the names at the moment
	shader = new Shader("shaders/particleVert.glsl", "shaders/particleFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	normalShader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (normalShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	//create the mesh
	sphere = new Mesh("meshes/Mario.obj");
	sphere->OnCreate();

	//sphereModelMatrix = MMath::translate(0.2, -0.08, 0.0f); //if you're going to do ANYTHING with the model matrix make sure it is done BEFORE the verts are loaded into the buffer

	std::srand(static_cast<unsigned int>(std::time(0)));

	if (camera.OnCreate() == false) { //create the camera 
		std::cout << "Camera failed ... we have a problem\n";
	}

	//create the buffer to hold the vertex positions between the vert stage and compute "stage"
	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * 10000, NULL, GL_DYNAMIC_COPY); //null canse the loadVertsToBuffer shader will handle it
	//the size of these buffers are bigger then they need to be because unless I deduplicate the mesh I can't get the true vertex count
	//thanks, artists

	//safest bet for the vertcount is multiply the listed size by 4 and then round up and you should have more than enough space
	//inefficient on storage/VRAM I know but I don't have any ideas on how to solve this problem

	//give all of our particles a starting velocity so they can orbit 
	Vec3 initialVelocities[10000];
	for (int i = 0; i < 10000; i++) {
		initialVelocities[i] = Vec3(0.0, 0.0, 0.0);
	}

	//store all those velocities in a buffere
	glGenBuffers(1, &velBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * 10000, initialVelocities, GL_DYNAMIC_COPY);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer); //bind the buffer
	glUseProgram(loadVertsToBuffer->GetProgram());
	glUniformMatrix4fv(loadVertsToBuffer->GetUniformID("projectionMatrix"), 1, GL_FALSE, camera.GetProjectionMatrix());
	glUniformMatrix4fv(loadVertsToBuffer->GetUniformID("viewMatrix"), 1, GL_FALSE, camera.GetViewMatrix());
	glUniformMatrix4fv(loadVertsToBuffer->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereModelMatrix);
	sphere->Render(GL_POINTS); //type of render doesn't really matter as there is nothing to see here
	glUseProgram(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); //unbind the buffer
	loadVertsToBuffer->OnDestroy(); //now that our vertices are safe and sound in the GPU we can delete this shader and free up the memory
	delete loadVertsToBuffer;
	//commented out scrap, I put the curly braces so I can collapse it down in visual studio
	{
		//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vec3) * 10000 , NULL, GL_DYNAMIC_COPY);  // Allocate memory but don't upload any data
		//marioModelMatrix = MMath::translate(0, 2, -2) * MMath::scale(0.11, 0.11, 0.11) * MMath::rotate(180, Vec3(0, 1, 0));

		/*glCreateTextures(GL_TEXTURE_2D, 1, &tex);
		glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureStorage2D(tex, 1, GL_RGBA32F, width, height);
		glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);*/


		/*glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
			GL_FLOAT, NULL);

		glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);*/



		//glCreateTextures(GL_TEXTURE_2D, 1, &verts);
		//glTextureParameteri(verts, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTextureParameteri(verts, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTextureParameteri(verts, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTextureParameteri(verts, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTextureStorage1D(verts, 1, GL_RGBA32F, sphere->vertices.size());
		//glBindImageTexture(0, verts, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		/*float size = sphere->vertices.size();
		glBindTexture(GL_TEXTURE_1D, verts);
		glUseProgram(vertexStoreShader->GetProgram());
		glUniform3fv(vertexStoreShader->GetUniformID("vert[0]"), size, &sphere->vertices[0].x);
		glDispatchCompute(size, 1, 1);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glUseProgram(0);*/



		//vertexStoreShader = new ComputeShader("shaders/storeVertsInTex.glsl");
		//if (vertexStoreShader->OnCreate() == false) {
		//	std::cout << "Shader failed ... we have a problem\n";
		//}


		//int viewport[4];
		//glGetIntegerv(GL_VIEWPORT, viewport);
		//SCREEN_WIDTH = viewport[2];
		//SCREEN_HEIGHT = viewport[3];


		
	}


	if (playerController.OnCreate() == false) {
		std::cout << "Controller failed ... we have a problem\n";
	}

	playerController.transform.setPos(0, 0, 2);

	ship = new Mesh("meshes/Midstone_Enemy.obj");
	ship->OnCreate();

	debris =  Model("Debris.obj");
	debris.OnCreate();

	
	explosion = new Explosion();
	explosion->OnCreate(&playerController.camera, loadVertsToBuffer, sphere, &debris);

	shipModelMatrix =   MMath::translate(0.0f,0.0f,0.0f) * MMath::rotate(90.0f, Vec3(0.0f, 1.0f, 0.0f)) ;

	sphereModelMatrix = MMath::rotate(90.0f, Vec3(0.0f, 1.0f, 0.0f)) ;



	//testLine = Line(Vec3(0, 0, 0), Vec3(0, 100, 0));
	
	lineShader = new Shader("shaders/lineVert.glsl", "shaders/lineFrag.glsl");
	if (lineShader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}



	return true;
}

void Scene5g::OnDestroy() {
	Debug::Info("Deleting assets Scene3p: ", __FILE__, __LINE__);

	normalShader->OnDestroy();
	delete normalShader;

	shader->OnDestroy();
	delete shader;

	sphere->OnDestroy();
	delete sphere;

	ship->OnDestroy();
	delete ship;

	debris.OnDestroy();

	computeShader->OnDestroy();
	delete computeShader;

	glDeleteBuffers(1, &posBuffer);
	glDeleteBuffers(1, &velBuffer);
}

void Scene5g::HandleEvents(const SDL_Event& sdlEvent) {
	playerController.handleEvents(sdlEvent);
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_L:
			printf("Reset!");
			explosion->ResetExplosion(resetParticles);
			break;
		}
	}
}

void Scene5g::Update(const float deltaTime) {
	playerController.Update(deltaTime);
	drawInWireMode = false;


	time += deltaTime;
	//only thing in update is to check frame performance to show how awesome the GPU is for being able to calculate 1500+ physics orbits each frame
	//I believe we're capped at 60 frames unless I mess with the component framework so I'm gonna go ahead and not do that
	float currentFrame = time;
	frameDelta = currentFrame - prevFrameTime; //get time between frames
	prevFrameTime = currentFrame; //set the previus frame to this frame for nect pass
	if (frameCounter > 500) { //print frame rate every 500 frames for performance reasons
		std::cout << "FPS: " << 1 / deltaTime << std::endl;
		frameCounter = 0;
	}
	else {
		frameCounter++;
	}
	shipPos.x -= deltaTime / 10;
	shipModelMatrix = MMath::translate(shipPos * 40) * MMath::rotate(90.0f, Vec3(0.0f, 1.0f, 0.0f));
	//sphereModelMatrix = MMath::translate(-shipPos) * MMath::rotate(90.0f, Vec3(0.0f, 1.0f, 0.0f));

	
	//sphereModelMatrix = shipModelMatrix;

	sphereModelMatrix = MMath::translate(Vec3(0,0,-20));


	explosion->Update(deltaTime);

}

void Scene5g::Render() {




	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//GLfloat lineVerticices[] =
	//{
	//	0,0,0,
	//	0,300,0
	//};
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, lineVerticices);
	//glUseProgram(lineShader->GetProgram());
	//glDrawArrays(GL_LINES, 0, 2);
	//glDisableClientState(GL_VERTEX_ARRAY);

	

	/*glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);*/

	/*glUseProgram(lineShader->GetProgram());
	glUniformMatrix4fv(lineShader->GetUniformID("MVP"), 1, GL_FALSE, testLine.MVP);
	glUniform4fv(lineShader->GetUniformID("color"), 1,  Vec4(0,1,0,1));
	testLine.draw();*/
	//glUseProgram(0);



	////bind the buffers
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuffer);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuffer);

	//if (frameCounter %	1 == 0) {
	//	glUseProgram(computeShader->GetProgram());

	//	glUniform1i(computeShader->GetUniformID("yDispatch"), 100); //amount of dispatches in the y direction(?) so the GPUs can work in parralel doing these calculations
	//	glUniform1f(computeShader->GetUniformID("simSpeed"), 60); //frequency 
	//	glUniform1f(computeShader->GetUniformID("randSeed"), time); //frequency 
	//	//glUniform3fv(computeShader->GetUniformID("forwardVector"), 1, Vec3(0,0,-1)); //direction the "ship" is headed
	//	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereModelMatrix);
	//	glDispatchCompute(100, 100, 1);//make sure the dispatch in the y parameter heres matches that in the uniform above
	//	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	//}
	//glUseProgram(shader->GetProgram());
	//glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	//glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	//glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphereModelMatrix);
	//sphere->Render(GL_POINTS);

	////unbind the buffers
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	//glUseProgram(0);


	//glUseProgram(normalShader->GetProgram());
	////glUniformMatrix4fv(normalShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	//glUniformMatrix4fv(normalShader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	//glUniformMatrix4fv(normalShader->GetUniformID("modelMatrix"), 1, GL_FALSE, shipModelMatrix);
	//glUniform4fv(normalShader->GetUniformID("meshColor"), 1, BLUE);
	//ship->Render(GL_TRIANGLES);

	/*glUseProgram(lineShader->GetProgram());
	glUniformMatrix4fv(lineShader->GetUniformID("projection"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	glUniformMatrix4fv(lineShader->GetUniformID("view"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	glUniformMatrix4fv(lineShader->GetUniformID("model"), 1, GL_FALSE, testLine.transform.toModelMatrix());
	testLine.draw();*/

	//glUseProgram(shader->GetProgram());
	////glUniformMatrix4fv(normalShader->GetUniformID("projectionMatrix"), 1, GL_FALSE, playerController.camera.GetProjectionMatrix());
	//glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, playerController.camera.GetViewMatrix());
	explosion->RenderDebris(normalShader);
	explosion->Render(shader, computeShader);
	

	glUseProgram(0);
	
}


//void Scene5g::draw_line(const Vec3& p1, const Vec3& p2, const Vec3& color)
//{
//	// point 1
//	lineData.push_back(p1.x);
//	lineData.push_back(p1.y);
//	lineData.push_back(p1.z);
//
//	// color
//	lineData.push_back(color.x);
//	lineData.push_back(color.y);
//	lineData.push_back(color.z);
//
//	// point 2
//	lineData.push_back(p2.x);
//	lineData.push_back(p2.y);
//	lineData.push_back(p2.z);
//
//	// color
//	lineData.push_back(color.x);
//	lineData.push_back(color.y);
//	lineData.push_back(color.z);
//
//}
//
//
//void Scene5g::draw_lines_flush()
//{
//	static unsigned int vao, vbo;
//
//	static bool created = false;
//	if (!created)
//	{
//		created = true;
//
//		glGenVertexArrays(1, &vao);
//		glBindVertexArray(vao);
//
//		glGenBuffers(1, &vbo);
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glBufferData(GL_ARRAY_BUFFER, lineData.size() * sizeof(float),
//			lineData.data(), GL_DYNAMIC_DRAW);
//
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
//			(void*)0);
//
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
//			(void*)(3 * sizeof(float)));
//	}
//	else
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
//		glBufferData(GL_ARRAY_BUFFER, lineData.size() * sizeof(float),
//			lineData.data(), GL_DYNAMIC_DRAW);
//	}
//
//	// 6 floats make up a vertex (3 position 3 color)
//	// divide by that to get number of vertices to draw
//	int count = lineData.size() / 6;
//
//	glBindVertexArray(vao);
//	glDrawArrays(GL_LINES, 0, count);
//
//	lineData.clear();
//}
//
//
//
