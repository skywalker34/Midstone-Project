#ifndef SCENE5G_H
#define SCENE5G_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include <vector>
#include "Trackball.h"
#include "Camera.h"
#include "PlayerController.h"
#include "Line.h"
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


	Shader* normalShader;

	Shader* shader = nullptr;

	float vertices[3] = {
	0.0f, 0.0f, -1.0f,


	};

	Shader* loadVertsToBuffer = nullptr;

	Shader* lineShader = nullptr;



	ComputeShader* computeShader = nullptr;

	Mesh* sphere = nullptr;
	Mesh* ship = nullptr;

	bool drawInWireMode = false;
	float time = 0;
	float prevFrameTime = 0.0f; //time of the previous frame
	float frameDelta = 0.0f; //time between frames

	Vec3 shipPos = Vec3(0, 0, 0);

	Camera camera;

	Line testLine;

	Matrix4 sphereModelMatrix;
	Matrix4 shipModelMatrix;

	PlayerController playerController;

	std::vector<float> lineData{};
	GLuint posBuffer;
	GLuint velBuffer;

	unsigned int VAO;

public:
	int frameCounter = 0;
	explicit Scene5g();
	virtual ~Scene5g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

};


#endif // SCENE4P_H