#pragma once

#include "types.hpp"

namespace Math3D {
void rotate(Vec3 &v, const Vec3 &sin, const Vec3 &cos);
void rotate(Tri3 &t, const Vec3 &sin, const Vec3 &cos);
bool project(const Vec3 &v, Vec2 &o);
bool project(const Tri3 &t, Tri2 &o);
} // namespace Math3D
