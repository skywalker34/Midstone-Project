#pragma once
#include <Vector.h> /// This is in GameDev
#include <Quaternion.h> /// This is in GameDev
using namespace MATH;

/// <summary>
/// Simple component class that acts as a container for a position, scale, and orientation.
/// </summary>
class Transform
{
private:
    Vec3 position;         // Position of the transform
    Quaternion orientation; // Orientation of the transform
    Vec3 scale;            // Scale of the transform
    Matrix4 parent;        // Parent matrix for hierarchical transformations

public:
    /// <summary>
    /// Constructor for Transform with position, orientation, and scale.
    /// </summary>
    /// <param name="_position">The position vector.</param>
    /// <param name="_orientation">The orientation quaternion.</param>
    /// <param name="_scale">The scale vector.</param>
    Transform(Vec3 _position, Quaternion _orientation, Vec3 _scale);

    /// <summary>
    /// Default constructor for Transform.
    /// </summary>
    Transform();

    // Setters

    /// <summary>
    /// Sets the position of the transform.
    /// </summary>
    /// <param name="p">The position vector.</param>
    void setPos(Vec3 p);

    /// <summary>
    /// Sets the position of the transform using individual float values.
    /// </summary>
    /// <param name="x">The x-coordinate.</param>
    /// <param name="y">The y-coordinate.</param>
    /// <param name="z">The z-coordinate.</param>
    void setPos(float x, float y, float z) {
        position = Vec3(x, y, z);
    }

    /// <summary>
    /// Sets the orientation of the transform.
    /// </summary>
    /// <param name="q">The orientation quaternion.</param>
    void setOrientation(Quaternion q);

    /// <summary>
    /// Sets the scale of the transform.
    /// </summary>
    /// <param name="s">The scale vector.</param>
    void setScale(Vec3 s);

    /// <summary>
    /// Sets the scale of the transform using individual float values.
    /// </summary>
    /// <param name="x">The x scale.</param>
    /// <param name="y">The y scale.</param>
    /// <param name="z">The z scale.</param>
    void setScale(float x, float y, float z) {
        scale = Vec3(x, y, z);
    }

    /// <summary>
    /// Sets the parent matrix for the transform.
    /// </summary>
    /// <param name="m">The parent matrix.</param>
    void setParent(Matrix4 m) {
        parent = m;
    }

    // Getters

    /// <summary>
    /// Gets the position of the transform.
    /// </summary>
    /// <returns>The position vector.</returns>
    Vec3 getPos() const;

    /// <summary>
    /// Gets the scale of the transform.
    /// </summary>
    /// <returns>The scale vector.</returns>
    Vec3 getScale() const;

    /// <summary>
    /// Gets the orientation of the transform.
    /// </summary>
    /// <returns>The orientation quaternion.</returns>
    Quaternion getOrientation() const;

    /// <summary>
    /// Builds a model matrix based on the transform.
    /// </summary>
    /// <returns>The model matrix.</returns>
    Matrix4 toModelMatrix() const;
};
