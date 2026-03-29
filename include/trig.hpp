#include "types.hpp"

// Fixed-point trigonometry
namespace Trig {
i16f16_t sin(i16f16_t v);
i16f16_t cos(i16f16_t v);
i16f16_t tan(i16f16_t v);

void sin(const Vec2 &a, Vec2 &o);
void cos(const Vec2 &a, Vec2 &o);
void tan(const Vec2 &a, Vec2 &o);

void sin(const Vec3 &a, Vec3 &o);
void cos(const Vec3 &a, Vec3 &o);
void tan(const Vec3 &a, Vec3 &o);

i16f16_t dot(const Vec2 &a, const Vec2 &b);
i16f16_t dot(const Vec3 &a, const Vec3 &b);

Vec3 cross(const Vec3 &a, const Vec3 &b);
} // namespace Trig