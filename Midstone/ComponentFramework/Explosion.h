#pragma once

#include "Model.h"
#include "Body.h"
#include "Transform.h"
#include "Camera.h"
#include "ComputeShader.h"

class Explosion
{

	Model* model;

	Mesh* mesh;
	Camera* cam;

	bool onExplode = true;

	float simTime = 3; //animation lasts 2 seconds
	float activeSimTime = 0;

	

	std::vector<Transform*> debrisTransforms;
	std::vector<Body*> body;

	GLuint posBuffer;
	GLuint velBuffer;

	const int YDISPATCH = 100;
	const int BUF_SIZE = 10000;

	Matrix4 modelMat;

	


public:
	
	int debrisChunkCount = 55;
	bool animComplete = true;

	bool OnCreate(Camera* cam_, Shader* loadVerts, Mesh* mesh_, Model* model_);
	void OnDestroy();

	void Update(float deltaTime);

	void OnExplode();
	void ResetExplosion(ComputeShader* comp);

	void RenderDebris(Shader* shader);
	void Render(Shader* shader, ComputeShader* comp);

	void setPos(Vec3 pos_);

	


};

