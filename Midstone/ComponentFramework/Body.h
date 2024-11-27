#ifndef BODY_H
#define BODY_H
#include <Vector.h> 
#include "Transform.h"
using namespace MATH; 


class Body {
public:
    Body();
	Body(Transform* tran, Vec3 vel_, Vec3 acc_, float mass_);

    ~Body();
// private: /// Physics stuff
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	Transform* transform;
	
	Vec3 angularVel;
	Quaternion orientation;

	float mass;
	const char* textureFile = "";
private: /// Graphics stuff 
	
	
public:
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void ApplyForce(Vec3 force);
	void setAccel(const Vec3 &accel_) { accel = accel_;}
	

	void UpdateOrientation(float deltaTime);


	const char* GetTextureFile() {
		return textureFile;
	}
	void SetTextureFile(const char* textureFile_) {
		textureFile = textureFile_;
	}
};

#endif
