#ifndef BODY_H
#define BODY_H
#include <Vector.h> /// This is in GameDev
#include "Transform.h"
using namespace MATH; 

/// Just forward declair these classes so I can define a pointer to them
/// Used later in the course



class Body {
public:
    Body();
	Body(Transform tran, Vec3 vel_, Vec3 acc_, float mass_);
    ~Body();
// private: /// Physics stuff
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	Transform transform;

	float mass;
	const char* textureFile = "";
private: /// Graphics stuff 
	
	
public:
	bool OnCreate();
	void OnDestroy();
	Transform Update(const float deltaTime, Transform t);
	void Render() const;
	void ApplyForce(Vec3 force);
	void setAccel(const Vec3 &accel_) { accel = accel_;}
	
	const char* GetTextureFile() {
		return textureFile;
	}
	void SetTextureFile(const char* textureFile_) {
		textureFile = textureFile_;
	}
};

#endif
