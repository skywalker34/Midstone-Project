#pragma once

#include "Model.h"
#include "Body.h"
#include "Transform.h"
#include "Camera.h"
#include "ComputeShader.h"
/// <summary>
/// Particle system to be triggered and lasts a set duration before going away.
/// </summary>
class Explosion
{
    Model* model; //reference to the larger model used for the debris chunks

    Mesh* mesh; //reference to the particle shader mesh

    Camera* cam; //reference to the camera for the compute shader

    bool onExplode = true;
    float simTime = 3; // Animation lasts 2 seconds
    float activeSimTime = 0; //how long the sim has been running

    std::vector<Transform*> debrisTransforms; //transform for all the debris chunks
    std::vector<Body*> body; //Bodies for all the larger debris chunks

    //open gl id
    GLuint posBuffer;//buffer to hold the positions of each part
    GLuint velBuffer; //buffer to hold the velocities of each particle

    const int YDISPATCH = 100; //dispatch in the y direction (allows for parrelel processing)
    const int BUF_SIZE = 10000; //size of the buffer (amount of point particles)

    Matrix4 modelMat; //model matrix for the particle mesh

public:
    int debrisChunkCount = 55; //count of larger mesh debris chunks
    bool animComplete = true; //whether or not the animation/simulation is currently running

    /// <summary>
    /// Initializes the explosion with the specified camera, shader, mesh, and model.
    /// </summary>
    /// <param name="cam_">Pointer to the camera object.</param>
    /// <param name="loadVerts">Pointer to the shader used for loading vertices.</param>
    /// <param name="mesh_">Pointer to the particle shader mesh object.</param>
    /// <param name="model_">Pointer to the model object for larger debris chunks.</param>
    /// <returns>Returns true if the creation is successful, otherwise false.</returns>
    bool OnCreate(Camera* cam_, Shader* loadVerts, Mesh* mesh_, Model* model_);

    /// <summary>
    /// Cleans up resources before destroying the explosion object.
    /// </summary>
    void OnDestroy();

    /// <summary>
    /// Updates the explosion state based on the elapsed time.
    /// </summary>
    /// <param name="deltaTime">The time elapsed since the last update.</param>
    void Update(float deltaTime);

    /// <summary>
    /// Triggers the explosion effect.
    /// </summary>
    void OnExplode();

    /// <summary>
    /// Resets the explosion state using the specified compute shader.
    /// </summary>
    /// <param name="comp">Pointer to the compute shader used for resetting the explosion.</param>
    void ResetExplosion(ComputeShader* comp);

    /// <summary>
    /// Renders the debris of the explosion using the specified shader.
    /// </summary>
    /// <param name="shader">Pointer to the shader used for rendering the debris.</param>
    void RenderDebris(Shader* shader);

    /// <summary>
    /// Renders the explosion using the specified shader and compute shader.
    /// </summary>
    /// <param name="shader">Pointer to the shader used for rendering.</param>
    /// <param name="comp">Pointer to the compute shader used for rendering.</param>
    void Render(Shader* shader, ComputeShader* comp);

    /// <summary>
    /// Sets the position of the explosion.
    /// </summary>
    /// <param name="pos_">The position vector to set.</param>
    void setPos(Vec3 pos_);
};
