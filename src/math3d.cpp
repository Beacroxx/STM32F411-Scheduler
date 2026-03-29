#include "math3d.hpp"

#include "sh1106.hpp"
#include "trig.hpp"

void Math3D::rotate(Vec3 &v, const Vec3 &sin, const Vec3 &cos) {
  i16f16_t x1 = cos.z * v.x - sin.z * v.y;
  i16f16_t y1 = sin.z * v.x + cos.z * v.y;

  i16f16_t z1 = -sin.y * x1 + cos.y * v.z;
  v.x = cos.y * x1 + sin.y * v.z;

  v.y = cos.x * y1 - sin.x * z1;
  v.z = sin.x * y1 + cos.x * z1;
}

void Math3D::rotate(Tri3 &t, const Vec3 &sin, const Vec3 &cos) {
  rotate(t.v1, sin, cos);
  rotate(t.v2, sin, cos);
  rotate(t.v3, sin, cos);
}

bool Math3D::project(const Vec3 &v, Vec2 &o) {
  constexpr i16f16_t DIST = i16f16_t(128);
  if (v.z + DIST <= 0)
    return false;

  i16f16_t scale = DIST / (v.z + DIST);
  o.x = v.x * scale + SH1106::WIDTH / 2;
  o.y = v.y * scale + SH1106::HEIGHT / 2;
  return true;
}

bool Math3D::project(const Tri3 &t, Tri2 &o) {
  return project(t.v1, o.v1) && project(t.v2, o.v2) && project(t.v3, o.v3);
}