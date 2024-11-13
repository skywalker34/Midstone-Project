#pragma once
#include "Vector.h"
#include <MMath.h>
#include <VMath.h>
#include <Matrix.h>
#include <glew.h>
#include <vector>

using namespace MATH;

class Line
{
public:
    int shaderProgram;
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    Vec3 startPoint;
    Vec3 endPoint;
    Matrix4 MVP;
    Vec3 lineColor;

    Line();
    Line(Vec3 start, Vec3 end);
    int setMVP(Matrix4 mvp);
    int setColor(Vec3 color);
    int draw();
    ~Line();
};