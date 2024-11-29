#include "Transform.h"
#include "MMath.h"

// Parameterized constructor for Transform
Transform::Transform(Vec3 _position, Quaternion _orientation, Vec3 _scale)
{
    position = _position;        // Set the position
    orientation = _orientation;  // Set the orientation
    scale = _scale;              // Set the scale
}

// Default constructor for Transform
Transform::Transform()
{
    // Default position to the origin
    position = Vec3(0.0f, 0.0f, 0.0f);
    // Default orientation to identity quaternion
    orientation = Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f));
    // Default scale to 1.0 in all axes
    scale = Vec3(1.0f, 1.0f, 1.0f);
}

// Set the position of the transform
void Transform::setPos(Vec3 p)
{
    position = p;
}

// Set the orientation of the transform
void Transform::setOrientation(Quaternion q)
{
    orientation = q;
}

// Set the scale of the transform
void Transform::setScale(Vec3 s)
{
    scale = s;
}

// Get the position of the transform
Vec3 Transform::getPos() const
{
    return position;
}

// Get the scale of the transform
Vec3 Transform::getScale() const
{
    return scale;
}

// Get the orientation of the transform
Quaternion Transform::getOrientation() const
{
    return orientation;
}

// Convert the transform to a model matrix
Matrix4 Transform::toModelMatrix() const
{
    // Create the rotation matrix from the orientation
    Matrix4 R = MMath::toMatrix4(orientation);
    // Create the translation matrix from the position
    Matrix4 T = MMath::translate(position);
    // Create the scale matrix from the scale
    Matrix4 S = MMath::scale(scale);

    // Combine the translation, rotation, and scale matrices to form the model matrix
    Matrix4 modelMat = T * R * S;
    // Apply any parent transformations
    modelMat = parent * modelMat;
    return modelMat;
}
