#pragma once
#include "Vector.h"
#include <Matrix.h>
#include <vector>
#include "glew.h"

using namespace MATH;

class Camera;
class Shader;
class Mesh;
class ComputeShader;

class ExhaustTrail
{
	Mesh* mesh;
	Camera* cam;

	GLuint posBuffer;
	GLuint velBuffer;

	const int YDISPATCH = 100;
	const int BUF_SIZE = 10000;

public:
		Matrix4 modelMat;


		bool OnCreate(Camera* cam_, Shader* loadVerts, Mesh* mesh_);
		void OnDestroy();
		void Render( Shader* shader, ComputeShader* comp) ;
		void Render(Shader* shader) const;
		void RunCompute(ComputeShader* comp);
};

