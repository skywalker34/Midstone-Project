#ifndef SCENE5G_H
#define SCENE5G_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include <vector>
#include "Trackball.h"
#include "Camera.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class softBody;
class Mesh;
class Shader;
class Texture;
class ComputeShader;


class Scene5g : public Scene {
private:


	Shader* shader = nullptr;

	Shader* loadVertsToBuffer = nullptr;

	ComputeShader* computeShader = nullptr;

	Mesh* sphere = nullptr;

	bool drawInWireMode = false;
	float time = 0;
	float prevFrameTime = 0.0f; //time of the previous frame
	float frameDelta = 0.0f; //time between frames


	Camera camera;

	Matrix4 sphereModelMatrix;
	Matrix4 marioModelMatrix;

	GLuint posBuffer;
	GLuint velBuffer;

public:
	int frameCounter = 0;
	explicit Scene5g();
	virtual ~Scene5g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE4P_H