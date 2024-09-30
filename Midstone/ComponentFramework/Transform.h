#pragma once
#include <Vector.h> /// This is in GameDev
#include <Quaternion.h> /// This is in GameDev
using namespace MATH;
//simple component class that acts as a container for a position, scale and orientation
class Transform
{
	private:
		Vec3 position;
		Quaternion orientation;
		Vec3 scale;

	public:
		//constructor
		Transform(Vec3 _position, Quaternion _orientation, Vec3 _scale);
		//Default constructor
		Transform();

		//Setters
		void setPos(Vec3 p);
		void setPos(float x, float y, float z) {
			position = Vec3(x, y, z);
		}
		void setOrientation(Quaternion q);
		void setScale(Vec3 s);
		void setScale(float x, float y, float z) {
			scale = Vec3(x, y, z);
		}
		//Getters
		Vec3 getPos() const;
		Vec3 getScale() const;
		Quaternion getOrientation() const;
		//builds a model matrix based off of a transform
		Matrix4 toModelMatrix() const;
};

