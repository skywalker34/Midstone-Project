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
/// <summary>
/// Particle system using compute shader to give the ship an exhaust trail jet stream
/// </summary>
class ExhaustTrail
{
    Mesh* mesh; // Reference to the particle mesh
    Camera* cam; // Reference to the camera

    // OpenGL IDs
    GLuint posBuffer; // Particle positions
    GLuint velBuffer; // Particle velocities

    const int YDISPATCH = 100; // Allows for parallel processing on the GPU
    const int BUF_SIZE = 10000; // Size of the buffers (how many particles)

public:
    Matrix4 modelMat; // Just a model matrix instead of a full transform so I can parent the exhaust trail to things

    /// <summary>
    /// Initializes the exhaust trail with the specified camera, shader, and mesh.
    /// </summary>
    /// <param name="cam_">Pointer to the camera object.</param>
    /// <param name="loadVerts">Pointer to the shader used for loading vertices.</param>
    /// <param name="mesh_">Pointer to the mesh object.</param>
    /// <returns>Returns true if the creation is successful, otherwise false.</returns>
    bool OnCreate(Camera* cam_, Shader* loadVerts, Mesh* mesh_);

    /// <summary>
    /// Cleans up resources before destroying the exhaust trail object.
    /// </summary>
    void OnDestroy();

    /// <summary>
    /// Runs the compute shader and renders the exhaust trail.
    /// </summary>
    /// <param name="shader">Pointer to the shader used for rendering.</param>
    /// <param name="comp">Pointer to the compute shader used for computing the spray effect.</param>
    void Render(Shader* shader, ComputeShader* comp);
};
