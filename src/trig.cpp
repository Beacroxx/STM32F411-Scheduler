#include "trig.hpp"

// 6th-order minimax for cos(x) in [-PI/2, PI/2]
// cos(x) ≈ 1 + x^2 * c1 + x^4 * c2 + x^6 * c3
// c1 ≈ -0.5, c2 ≈ 0.04166368, c3 ≈ -0.00138537
static constexpr i16f16_t c1 = i16f16_t(-0.5f);
static constexpr i16f16_t c2 = i16f16_t(0.04166368f);
static constexpr i16f16_t c3 = i16f16_t(-0.00138537f);

i16f16_t Trig::sin(i16f16_t v) { return Trig::cos(v - F_CONST::PI_2); }

i16f16_t Trig::cos(i16f16_t v) {
  i16f16_t x = v;
  while (x > F_CONST::PI)
    x = x - F_CONST::TWOPI;
  while (x < -F_CONST::PI)
    x = x + F_CONST::TWOPI;
  int sign = 1;
  i16f16_t out_x;
  if (x > F_CONST::PI_2) {
    out_x = F_CONST::PI - x;
    sign = -1;
  } else if (x < -F_CONST::PI_2) {
    out_x = -F_CONST::PI - x;
    sign = -1;
  } else {
    out_x = x;
  }
  x = out_x;
  i16f16_t x2 = x * x;
  i16f16_t x4 = x2 * x2;
  i16f16_t x6 = x4 * x2;
  i16f16_t result = i16f16_t(1.0f) + x2 * c1 + x4 * c2 + x6 * c3;
  return sign * result;
}

i16f16_t Trig::tan(i16f16_t v) {
  // Use range reduction for better accuracy
  i16f16_t s = Trig::sin(v);
  i16f16_t c = Trig::cos(v);
  constexpr i16f16_t epsilon = i16f16_t(1, true);
  if (c > -epsilon && c < epsilon) {
    return (s > i16f16_t(0)) ? i16f16_t(32768) : i16f16_t(-32767);
  }
  return s / c;
}

void Trig::sin(const Vec2 &a, Vec2 &o) {
  o.x = sin(a.x);
  o.y = sin(a.y);
}

void Trig::cos(const Vec2 &a, Vec2 &o) {
  o.x = cos(a.x);
  o.y = cos(a.y);
}

void Trig::tan(const Vec2 &a, Vec2 &o) {
  o.x = tan(a.x);
  o.y = tan(a.y);
}

void Trig::sin(const Vec3 &a, Vec3 &o) {
  o.x = sin(a.x);
  o.y = sin(a.y);
  o.z = sin(a.z);
}

void Trig::cos(const Vec3 &a, Vec3 &o) {
  o.x = cos(a.x);
  o.y = cos(a.y);
  o.z = cos(a.z);
}

void Trig::tan(const Vec3 &a, Vec3 &o) {
  o.x = tan(a.x);
  o.y = tan(a.y);
  o.z = tan(a.z);
}

i16f16_t Trig::dot(const Vec2 &a, const Vec2 &b) { return a.x * b.x + a.y * b.y; }
i16f16_t Trig::dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec3 Trig::cross(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}