#ifndef SPHERE_H
#define SPHERE_H
#include <Vector.h> /// This is in GameDev
using namespace MATH; 

/// Just forward declair these classes so I can define a pointer to them
/// Used later in the course
class Mesh;
class Texture;

class Sphere {
public:
	Sphere();
	Sphere(Vec3 transform_, float radius_);
    ~Sphere();
// private: /// Physics stuff
	Vec3 transform;
	float radius;
private: /// Graphics stuff 
	//Mesh *mesh;
	//Texture *texture;
	
public:
	bool OnCreate();
	void OnDestroy();
	void Update(float deltaTime);
	//void Render() const;
};

#endif
