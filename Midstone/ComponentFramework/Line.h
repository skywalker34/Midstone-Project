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


    Transform transform;
    unsigned int vbo;
    unsigned int vao;
    float lineThickness = 5.0f;
    float vertices[3] = {
     0.0f, 0.0f, -1.0f,
    };

    Line();
    Line(Vec3 start, Vec3 end);
    void RecalculateLine(Vec3 start, Vec3 end);
    void draw();
    ~Line();
};