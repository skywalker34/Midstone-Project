#pragma once
#include <Vector.h>

#ifndef CONSTANTS
#define CONSTANTS
using namespace MATH;

const Vec3 ORIGIN = Vec3(0, 0, 0);

const float CAMERA_TO_ORIGIN = 20;	// Remove or rename

const float PLANET_RADIUS = 30.0f;

const int PLANET_HEALTH = 50;

// Direction
const Vec3 FORWARD = Vec3(0, 0, -1);
const Vec3 BACKWARD = Vec3(0, 0, 1);
const Vec3 LEFT = Vec3(-1, 0, 0);
const Vec3 RIGHT = Vec3(1, 0, 0);
const Vec3 UP = Vec3(0, 1, 0);
const Vec3 DOWN = Vec3(0, -1, 0);

// Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Colours
const Vec4 RED = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
const Vec4 GREEN = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
const Vec4 BLUE = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
const Vec4 ORANGE = Vec4(1, 0.6, 0.0f, 1.0f);
const Vec4 WHITE = Vec4(1, 1, 1, 1);
const Vec4 PURPLE = Vec4(0.5f, 0.0f, 0.5f, 1.0f);
const Vec4 GREY = Vec4(0.2f, 0.2f, 0.2f, 1.0f);

#endif