#pragma once

#include "types.hpp"

namespace Math3D {

/**
 * @brief Rotate a 3D vector by given sine and cosine values.
 * @param v Vector to rotate (in/out).
 * @param sin Sine values for each axis.
 * @param cos Cosine values for each axis.
 */
void rotate(Vec3 &v, const Vec3 &sin, const Vec3 &cos);

/**
 * @brief Rotate a 3D triangle by given sine and cosine values.
 * @param t Triangle to rotate (in/out).
 * @param sin Sine values for each axis.
 * @param cos Cosine values for each axis.
 */
void rotate(Tri3 &t, const Vec3 &sin, const Vec3 &cos);

/**
 * @brief Project a 3D vector to 2D.
 * @param v Input 3D vector.
 * @param o Output 2D vector.
 * @return true if projection succeeded, false otherwise.
 */
bool project(const Vec3 &v, Vec2 &o);

/**
 * @brief Project a 3D triangle to 2D.
 * @param t Input 3D triangle.
 * @param o Output 2D triangle.
 * @return true if projection succeeded, false otherwise.
 */
bool project(const Tri3 &t, Tri2 &o);
} // namespace Math3D
