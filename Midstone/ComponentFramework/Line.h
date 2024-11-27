#pragma once
#include "Vector.h"
#include <MMath.h>
#include <VMath.h>
#include <Matrix.h>
#include <glew.h>
#include <vector>
#include "Transform.h"

using namespace MATH;

class Line
{
public:
    //transform for the line in the world
    Transform transform;
    //Open gl ID's
    unsigned int vbo;
    unsigned int vao;

    float lineThickness = 5.0f; //thickness of th eline for rendering

    //Vertex points for the line 
   //in local space a line goes from the origin ato 0,0,-1
    float vertices[3] = {
        0.0f, 0.0f, -1.0f,
    };

    /// <summary>
    /// Default constructor for the Line class.
    /// </summary>
    Line();

    /// <summary>
    /// Constructor for the Line class that initializes the start and end points of the line.
    /// </summary>
    /// <param name="start">The starting point of the line (in world space).</param>
    /// <param name="end">The ending point of the line (in world space).</param>
    Line(Vec3 start, Vec3 end);

    /// <summary>
    /// After a line has been created, allows you to change its start and end positions.
    /// </summary>
    /// <param name="start">The point (in world space) where the line starts.</param>
    /// <param name="end">The point (in world space) where the line ends.</param>
    void RecalculateLine(Vec3 start, Vec3 end);

    /// <summary>
    /// Draws the line.
    /// </summary>
    void draw();

    /// <summary>
    /// Destructor for the Line class.
    /// </summary>
    ~Line();
};
