#ifndef COLLISION_H
#define COLLISION_H
#include "Sphere.h"
#include "DQMath.h"

//From Game Physics
using namespace MATHEX;
using namespace MATH;

namespace COLLISION {
    /// <summary>
    /// Checks if two spheres have collided and returns true or false.
    /// </summary>
    /// <param name="body1">Pointer to the first sphere.</param>
    /// <param name="body2">Pointer to the second sphere.</param>
    /// <returns>Returns true if the spheres have collided, otherwise false.</returns>
    bool SphereSphereCollisionDetected(const Sphere* body1, const Sphere* body2);

    /// <summary>
    /// Checks if a line (defined as a dual quaternion) and a sphere have collided.
    /// </summary>
    /// <param name="body">Pointer to the sphere.</param>
    /// <param name="line">The dual quaternion representing the line.</param>
    /// <returns>Returns true if the line and sphere have collided, otherwise false.</returns>
    bool LineSphereCollisionDetected(const Sphere* body, const DualQuat line);

    /// <summary>
    /// Checks if a point (in space) and a sphere have collided.
    /// </summary>
    /// <param name="body">Pointer to the sphere.</param>
    /// <param name="pos">The position of the point in space.</param>
    /// <returns>Returns true if the point and sphere have collided, otherwise false.</returns>
    bool SpherePointCollisionDetected(const Sphere* body, const Vec3 pos);
}


#endif
