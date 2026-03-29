#include "types.hpp"

// Fixed-point trigonometry
namespace Trig {

/**
 * @brief Compute sine of a fixed-point value.
 * @param v Input value.
 * @return Sine of v.
 */
i16f16_t sin(i16f16_t v);

/**
 * @brief Compute cosine of a fixed-point value.
 * @param v Input value.
 * @return Cosine of v.
 */
i16f16_t cos(i16f16_t v);

/**
 * @brief Compute tangent of a fixed-point value.
 * @param v Input value.
 * @return Tangent of v.
 */
i16f16_t tan(i16f16_t v);

/**
 * @brief Compute sine of a 2D vector.
 * @param a Input vector.
 * @param o Output vector (sine values).
 */
void sin(const Vec2 &a, Vec2 &o);

/**
 * @brief Compute cosine of a 2D vector.
 * @param a Input vector.
 * @param o Output vector (cosine values).
 */
void cos(const Vec2 &a, Vec2 &o);

/**
 * @brief Compute tangent of a 2D vector.
 * @param a Input vector.
 * @param o Output vector (tangent values).
 */
void tan(const Vec2 &a, Vec2 &o);

/**
 * @brief Compute sine of a 3D vector.
 * @param a Input vector.
 * @param o Output vector (sine values).
 */
void sin(const Vec3 &a, Vec3 &o);

/**
 * @brief Compute cosine of a 3D vector.
 * @param a Input vector.
 * @param o Output vector (cosine values).
 */
void cos(const Vec3 &a, Vec3 &o);

/**
 * @brief Compute tangent of a 3D vector.
 * @param a Input vector.
 * @param o Output vector (tangent values).
 */
void tan(const Vec3 &a, Vec3 &o);

/**
 * @brief Compute dot product of two 2D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Dot product.
 */
i16f16_t dot(const Vec2 &a, const Vec2 &b);

/**
 * @brief Compute dot product of two 3D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Dot product.
 */
i16f16_t dot(const Vec3 &a, const Vec3 &b);

/**
 * @brief Compute cross product of two 3D vectors.
 * @param a First vector.
 * @param b Second vector.
 * @return Cross product vector.
 */
Vec3 cross(const Vec3 &a, const Vec3 &b);
} // namespace Trig