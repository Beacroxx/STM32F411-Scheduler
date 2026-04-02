#pragma once

#include "memorymanager.hpp"

#include <cmath>
#include <cstdint>

/**
 * @brief Fixed-point 16.16 signed integer type.
 */
struct i16f16_t {
  int32_t value;

  // Constructors

  i16f16_t() = default;
  constexpr explicit i16f16_t(int32_t v) : value(static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  consteval explicit i16f16_t(const float v) : value(static_cast<int32_t>(v * 65536.0f)) {}
  consteval explicit i16f16_t(const double v) : value(static_cast<int32_t>(v * 65536.0f)) {}
  constexpr i16f16_t(int v) : value(static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  constexpr i16f16_t(int16_t v) : value(static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  constexpr i16f16_t(uint16_t v) : value(static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  constexpr i16f16_t(int32_t v, bool raw) : value(raw ? v : static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  constexpr i16f16_t(uint32_t v) : value(static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  constexpr i16f16_t(int64_t v) : value(static_cast<int32_t>(v * 65536)) {}
  constexpr i16f16_t(uint64_t v) : value(static_cast<int32_t>(v * 65536)) {}

  // Conversions to common types (all shift by 16 for int types)

  constexpr explicit operator int() const { return value / 65536; }
  constexpr explicit operator int16_t() const { return static_cast<int16_t>(value / 65536); }
  constexpr explicit operator uint16_t() const { return static_cast<uint16_t>(value / 65536); }
  constexpr explicit operator int32_t() const { return value / 65536; }
  constexpr explicit operator uint32_t() const { return static_cast<uint32_t>(value / 65536); }
  constexpr explicit operator int64_t() const { return static_cast<int64_t>(value) / 65536; }
  constexpr explicit operator uint64_t() const { return static_cast<uint64_t>(value) / 65536; }

  // Raw value access
  constexpr int32_t raw() const { return value; }
};

// Common constants
namespace F_CONST {
constexpr i16f16_t PI = i16f16_t(static_cast<float>(M_PI));
constexpr i16f16_t TWOPI = i16f16_t(static_cast<float>(M_TWOPI));
constexpr i16f16_t E = i16f16_t(static_cast<float>(M_E));
constexpr i16f16_t PHI = i16f16_t(static_cast<float>(1.6180339887498948482));
constexpr i16f16_t LOG2E = i16f16_t(static_cast<float>(M_LOG2E));
constexpr i16f16_t LOG10E = i16f16_t(static_cast<float>(M_LOG10E));
constexpr i16f16_t LN2 = i16f16_t(static_cast<float>(M_LN2));
constexpr i16f16_t LN10 = i16f16_t(static_cast<float>(M_LN10));
constexpr i16f16_t PI_2 = i16f16_t(static_cast<float>(M_PI_2));
constexpr i16f16_t PI_4 = i16f16_t(static_cast<float>(M_PI_4));
constexpr i16f16_t _1_PI = i16f16_t(static_cast<float>(M_1_PI));
constexpr i16f16_t _2_PI = i16f16_t(static_cast<float>(M_2_PI));
constexpr i16f16_t _2_SQRTPI = i16f16_t(static_cast<float>(M_2_SQRTPI));
constexpr i16f16_t SQRT2 = i16f16_t(static_cast<float>(M_SQRT2));
constexpr i16f16_t SQRT3 = i16f16_t(static_cast<float>(M_SQRT3));
constexpr i16f16_t SQRT1_2 = i16f16_t(static_cast<float>(M_SQRT1_2));
} // namespace F_CONST

// Assignment operators

constexpr i16f16_t &operator+=(i16f16_t &a, i16f16_t b) {
  a.value += b.value;
  return a;
}
constexpr i16f16_t &operator-=(i16f16_t &a, i16f16_t b) {
  a.value -= b.value;
  return a;
}
constexpr i16f16_t &operator*=(i16f16_t &a, i16f16_t b) {
  a.value = static_cast<int32_t>((static_cast<int64_t>(a.value) * b.value) >> 16);
  return a;
}
constexpr i16f16_t &operator/=(i16f16_t &a, i16f16_t b) {
  a.value = static_cast<int32_t>((static_cast<int64_t>(a.value) << 16) / b.value);
  return a;
}
constexpr i16f16_t &operator%=(i16f16_t &a, i16f16_t b) {
  a.value = a.value % b.value;
  return a;
}

// Binary operators

constexpr i16f16_t operator+(i16f16_t a, i16f16_t b) { return i16f16_t(a.value + b.value, true); }
constexpr i16f16_t operator-(i16f16_t a, i16f16_t b) { return i16f16_t(a.value - b.value, true); }
constexpr i16f16_t operator*(i16f16_t a, i16f16_t b) {
  return i16f16_t(static_cast<int32_t>((static_cast<int64_t>(a.value) * b.value) >> 16), true);
}
constexpr i16f16_t operator/(i16f16_t a, i16f16_t b) {
  return i16f16_t(static_cast<int32_t>((static_cast<int64_t>(a.value) << 16) / b.value), true);
}
constexpr i16f16_t operator%(i16f16_t a, i16f16_t b) { return i16f16_t(a.value % b.value, true); }

// Unary operator

constexpr i16f16_t operator-(i16f16_t a) { return i16f16_t(-a.value, true); }
constexpr i16f16_t operator+(i16f16_t a) { return i16f16_t(a.value, true); }

// Comparisons

constexpr bool operator==(i16f16_t a, i16f16_t b) { return a.value == b.value; }
constexpr bool operator!=(i16f16_t a, i16f16_t b) { return a.value != b.value; }
constexpr bool operator<(i16f16_t a, i16f16_t b) { return a.value < b.value; }
constexpr bool operator<=(i16f16_t a, i16f16_t b) { return a.value <= b.value; }
constexpr bool operator>(i16f16_t a, i16f16_t b) { return a.value > b.value; }
constexpr bool operator>=(i16f16_t a, i16f16_t b) { return a.value >= b.value; }

// Mixed-type arithmetic (integral types to i16f16_t)

// int
constexpr i16f16_t operator+(i16f16_t a, int b) { return a + i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator+(int a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) + b; }
constexpr i16f16_t operator-(i16f16_t a, int b) { return a - i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator-(int a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) - b; }
constexpr i16f16_t operator*(i16f16_t a, int b) { return a * i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator*(int a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) * b; }
constexpr i16f16_t operator/(i16f16_t a, int b) { return a / i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator/(int a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) / b; }
constexpr i16f16_t operator%(i16f16_t a, int b) { return a % i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator%(int a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) % b; }

// int16_t
constexpr i16f16_t operator+(i16f16_t a, int16_t b) { return a + i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator+(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) + b; }
constexpr i16f16_t operator-(i16f16_t a, int16_t b) { return a - i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator-(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) - b; }
constexpr i16f16_t operator*(i16f16_t a, int16_t b) { return a * i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator*(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) * b; }
constexpr i16f16_t operator/(i16f16_t a, int16_t b) { return a / i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator/(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) / b; }
constexpr i16f16_t operator%(i16f16_t a, int16_t b) { return a % i16f16_t(static_cast<int32_t>(b)); }
constexpr i16f16_t operator%(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int32_t>(a)) % b; }

// uint16_t
constexpr i16f16_t operator+(i16f16_t a, uint16_t b) { return a + i16f16_t(static_cast<uint32_t>(b)); }
constexpr i16f16_t operator+(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<uint32_t>(a)) + b; }
constexpr i16f16_t operator-(i16f16_t a, uint16_t b) { return a - i16f16_t(static_cast<uint32_t>(b)); }
constexpr i16f16_t operator-(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<uint32_t>(a)) - b; }
constexpr i16f16_t operator*(i16f16_t a, uint16_t b) { return a * i16f16_t(static_cast<uint32_t>(b)); }
constexpr i16f16_t operator*(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<uint32_t>(a)) * b; }
constexpr i16f16_t operator/(i16f16_t a, uint16_t b) { return a / i16f16_t(static_cast<uint32_t>(b)); }
constexpr i16f16_t operator/(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<uint32_t>(a)) / b; }
constexpr i16f16_t operator%(i16f16_t a, uint16_t b) { return a % i16f16_t(static_cast<uint32_t>(b)); }
constexpr i16f16_t operator%(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<uint32_t>(a)) % b; }

// int32_t
constexpr i16f16_t operator+(i16f16_t a, int32_t b) { return a + i16f16_t(b); }
constexpr i16f16_t operator+(int32_t a, i16f16_t b) { return i16f16_t(a) + b; }
constexpr i16f16_t operator-(i16f16_t a, int32_t b) { return a - i16f16_t(b); }
constexpr i16f16_t operator-(int32_t a, i16f16_t b) { return i16f16_t(a) - b; }
constexpr i16f16_t operator*(i16f16_t a, int32_t b) { return a * i16f16_t(b); }
constexpr i16f16_t operator*(int32_t a, i16f16_t b) { return i16f16_t(a) * b; }
constexpr i16f16_t operator/(i16f16_t a, int32_t b) { return a / i16f16_t(b); }
constexpr i16f16_t operator/(int32_t a, i16f16_t b) { return i16f16_t(a) / b; }
constexpr i16f16_t operator%(i16f16_t a, int32_t b) { return a % i16f16_t(b); }
constexpr i16f16_t operator%(int32_t a, i16f16_t b) { return i16f16_t(a) % b; }

// uint32_t
constexpr i16f16_t operator+(i16f16_t a, uint32_t b) { return a + i16f16_t(b); }
constexpr i16f16_t operator+(uint32_t a, i16f16_t b) { return i16f16_t(a) + b; }
constexpr i16f16_t operator-(i16f16_t a, uint32_t b) { return a - i16f16_t(b); }
constexpr i16f16_t operator-(uint32_t a, i16f16_t b) { return i16f16_t(a) - b; }
constexpr i16f16_t operator*(i16f16_t a, uint32_t b) { return a * i16f16_t(b); }
constexpr i16f16_t operator*(uint32_t a, i16f16_t b) { return i16f16_t(a) * b; }
constexpr i16f16_t operator/(i16f16_t a, uint32_t b) { return a / i16f16_t(b); }
constexpr i16f16_t operator/(uint32_t a, i16f16_t b) { return i16f16_t(a) / b; }
constexpr i16f16_t operator%(i16f16_t a, uint32_t b) { return a % i16f16_t(b); }
constexpr i16f16_t operator%(uint32_t a, i16f16_t b) { return i16f16_t(a) % b; }

// int64_t
constexpr i16f16_t operator+(i16f16_t a, int64_t b) { return a + i16f16_t(b); }
constexpr i16f16_t operator+(int64_t a, i16f16_t b) { return i16f16_t(a) + b; }
constexpr i16f16_t operator-(i16f16_t a, int64_t b) { return a - i16f16_t(b); }
constexpr i16f16_t operator-(int64_t a, i16f16_t b) { return i16f16_t(a) - b; }
constexpr i16f16_t operator*(i16f16_t a, int64_t b) { return a * i16f16_t(b); }
constexpr i16f16_t operator*(int64_t a, i16f16_t b) { return i16f16_t(a) * b; }
constexpr i16f16_t operator/(i16f16_t a, int64_t b) { return a / i16f16_t(b); }
constexpr i16f16_t operator/(int64_t a, i16f16_t b) { return i16f16_t(a) / b; }
constexpr i16f16_t operator%(i16f16_t a, int64_t b) { return a % i16f16_t(b); }
constexpr i16f16_t operator%(int64_t a, i16f16_t b) { return i16f16_t(a) % b; }

// uint64_t
constexpr i16f16_t operator+(i16f16_t a, uint64_t b) { return a + i16f16_t(b); }
constexpr i16f16_t operator+(uint64_t a, i16f16_t b) { return i16f16_t(a) + b; }
constexpr i16f16_t operator-(i16f16_t a, uint64_t b) { return a - i16f16_t(b); }
constexpr i16f16_t operator-(uint64_t a, i16f16_t b) { return i16f16_t(a) - b; }
constexpr i16f16_t operator*(i16f16_t a, uint64_t b) { return a * i16f16_t(b); }
constexpr i16f16_t operator*(uint64_t a, i16f16_t b) { return i16f16_t(a) * b; }
constexpr i16f16_t operator/(i16f16_t a, uint64_t b) { return a / i16f16_t(b); }
constexpr i16f16_t operator/(uint64_t a, i16f16_t b) { return i16f16_t(a) / b; }
constexpr i16f16_t operator%(i16f16_t a, uint64_t b) { return a % i16f16_t(b); }
constexpr i16f16_t operator%(uint64_t a, i16f16_t b) { return i16f16_t(a) % b; }

// Prevent accidental float/double overloads

template <typename T> constexpr i16f16_t operator+(i16f16_t, T) = delete;
template <typename T> constexpr i16f16_t operator+(T, i16f16_t) = delete;
template <typename T> constexpr i16f16_t operator-(i16f16_t, T) = delete;
template <typename T> constexpr i16f16_t operator-(T, i16f16_t) = delete;
template <typename T> constexpr i16f16_t operator*(i16f16_t, T) = delete;
template <typename T> constexpr i16f16_t operator*(T, i16f16_t) = delete;
template <typename T> constexpr i16f16_t operator/(i16f16_t, T) = delete;
template <typename T> constexpr i16f16_t operator/(T, i16f16_t) = delete;

// Mixed-type comparisons (integral types)

// int
constexpr bool operator==(i16f16_t a, int b) { return a == i16f16_t(b); }
constexpr bool operator==(int a, i16f16_t b) { return i16f16_t(a) == b; }
constexpr bool operator!=(i16f16_t a, int b) { return a != i16f16_t(b); }
constexpr bool operator!=(int a, i16f16_t b) { return i16f16_t(a) != b; }
constexpr bool operator<(i16f16_t a, int b) { return a < i16f16_t(b); }
constexpr bool operator<(int a, i16f16_t b) { return i16f16_t(a) < b; }
constexpr bool operator<=(i16f16_t a, int b) { return a <= i16f16_t(b); }
constexpr bool operator<=(int a, i16f16_t b) { return i16f16_t(a) <= b; }
constexpr bool operator>(i16f16_t a, int b) { return a > i16f16_t(b); }
constexpr bool operator>(int a, i16f16_t b) { return i16f16_t(a) > b; }
constexpr bool operator>=(i16f16_t a, int b) { return a >= i16f16_t(b); }
constexpr bool operator>=(int a, i16f16_t b) { return i16f16_t(a) >= b; }

// int16_t
constexpr bool operator==(i16f16_t a, int16_t b) { return a == i16f16_t(static_cast<int>(b)); }
constexpr bool operator==(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) == b; }
constexpr bool operator!=(i16f16_t a, int16_t b) { return a != i16f16_t(static_cast<int>(b)); }
constexpr bool operator!=(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) != b; }
constexpr bool operator<(i16f16_t a, int16_t b) { return a < i16f16_t(static_cast<int>(b)); }
constexpr bool operator<(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) < b; }
constexpr bool operator<=(i16f16_t a, int16_t b) { return a <= i16f16_t(static_cast<int>(b)); }
constexpr bool operator<=(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) <= b; }
constexpr bool operator>(i16f16_t a, int16_t b) { return a > i16f16_t(static_cast<int>(b)); }
constexpr bool operator>(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) > b; }
constexpr bool operator>=(i16f16_t a, int16_t b) { return a >= i16f16_t(static_cast<int>(b)); }
constexpr bool operator>=(int16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) >= b; }

// uint16_t
constexpr bool operator==(i16f16_t a, uint16_t b) { return a == i16f16_t(static_cast<int>(b)); }
constexpr bool operator==(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) == b; }
constexpr bool operator!=(i16f16_t a, uint16_t b) { return a != i16f16_t(static_cast<int>(b)); }
constexpr bool operator!=(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) != b; }
constexpr bool operator<(i16f16_t a, uint16_t b) { return a < i16f16_t(static_cast<int>(b)); }
constexpr bool operator<(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) < b; }
constexpr bool operator<=(i16f16_t a, uint16_t b) { return a <= i16f16_t(static_cast<int>(b)); }
constexpr bool operator<=(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) <= b; }
constexpr bool operator>(i16f16_t a, uint16_t b) { return a > i16f16_t(static_cast<int>(b)); }
constexpr bool operator>(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) > b; }
constexpr bool operator>=(i16f16_t a, uint16_t b) { return a >= i16f16_t(static_cast<int>(b)); }
constexpr bool operator>=(uint16_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) >= b; }

// int32_t
constexpr bool operator==(i16f16_t a, int32_t b) { return a == i16f16_t(static_cast<int>(b)); }
constexpr bool operator==(int32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) == b; }
constexpr bool operator!=(i16f16_t a, int32_t b) { return a != i16f16_t(static_cast<int>(b)); }
constexpr bool operator!=(int32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) != b; }
constexpr bool operator<(i16f16_t a, int32_t b) { return a < i16f16_t(static_cast<int>(b)); }
constexpr bool operator<(int32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) < b; }
constexpr bool operator<=(i16f16_t a, int32_t b) { return a <= i16f16_t(static_cast<int>(b)); }
constexpr bool operator<=(int32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) <= b; }
constexpr bool operator>(i16f16_t a, int32_t b) { return a > i16f16_t(static_cast<int>(b)); }
constexpr bool operator>(int32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) > b; }
constexpr bool operator>=(i16f16_t a, int32_t b) { return a >= i16f16_t(static_cast<int>(b)); }
constexpr bool operator>=(int32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) >= b; }

// uint32_t
constexpr bool operator==(i16f16_t a, uint32_t b) { return a == i16f16_t(static_cast<int>(b)); }
constexpr bool operator==(uint32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) == b; }
constexpr bool operator!=(i16f16_t a, uint32_t b) { return a != i16f16_t(static_cast<int>(b)); }
constexpr bool operator!=(uint32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) != b; }
constexpr bool operator<(i16f16_t a, uint32_t b) { return a < i16f16_t(static_cast<int>(b)); }
constexpr bool operator<(uint32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) < b; }
constexpr bool operator<=(i16f16_t a, uint32_t b) { return a <= i16f16_t(static_cast<int>(b)); }
constexpr bool operator<=(uint32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) <= b; }
constexpr bool operator>(i16f16_t a, uint32_t b) { return a > i16f16_t(static_cast<int>(b)); }
constexpr bool operator>(uint32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) > b; }
constexpr bool operator>=(i16f16_t a, uint32_t b) { return a >= i16f16_t(static_cast<int>(b)); }
constexpr bool operator>=(uint32_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) >= b; }

// int64_t
constexpr bool operator==(i16f16_t a, int64_t b) { return a == i16f16_t(static_cast<int>(b)); }
constexpr bool operator==(int64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) == b; }
constexpr bool operator!=(i16f16_t a, int64_t b) { return a != i16f16_t(static_cast<int>(b)); }
constexpr bool operator!=(int64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) != b; }
constexpr bool operator<(i16f16_t a, int64_t b) { return a < i16f16_t(static_cast<int>(b)); }
constexpr bool operator<(int64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) < b; }
constexpr bool operator<=(i16f16_t a, int64_t b) { return a <= i16f16_t(static_cast<int>(b)); }
constexpr bool operator<=(int64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) <= b; }
constexpr bool operator>(i16f16_t a, int64_t b) { return a > i16f16_t(static_cast<int>(b)); }
constexpr bool operator>(int64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) > b; }
constexpr bool operator>=(i16f16_t a, int64_t b) { return a >= i16f16_t(static_cast<int>(b)); }
constexpr bool operator>=(int64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) >= b; }

// uint64_t
constexpr bool operator==(i16f16_t a, uint64_t b) { return a == i16f16_t(static_cast<int>(b)); }
constexpr bool operator==(uint64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) == b; }
constexpr bool operator!=(i16f16_t a, uint64_t b) { return a != i16f16_t(static_cast<int>(b)); }
constexpr bool operator!=(uint64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) != b; }
constexpr bool operator<(i16f16_t a, uint64_t b) { return a < i16f16_t(static_cast<int>(b)); }
constexpr bool operator<(uint64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) < b; }
constexpr bool operator<=(i16f16_t a, uint64_t b) { return a <= i16f16_t(static_cast<int>(b)); }
constexpr bool operator<=(uint64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) <= b; }
constexpr bool operator>(i16f16_t a, uint64_t b) { return a > i16f16_t(static_cast<int>(b)); }
constexpr bool operator>(uint64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) > b; }
constexpr bool operator>=(i16f16_t a, uint64_t b) { return a >= i16f16_t(static_cast<int>(b)); }
constexpr bool operator>=(uint64_t a, i16f16_t b) { return i16f16_t(static_cast<int>(a)) >= b; }

// Prevent accidental float/double comparisons

template <typename T> constexpr bool operator==(i16f16_t, T) = delete;
template <typename T> constexpr bool operator==(T, i16f16_t) = delete;
template <typename T> constexpr bool operator!=(i16f16_t, T) = delete;
template <typename T> constexpr bool operator!=(T, i16f16_t) = delete;
template <typename T> constexpr bool operator<(i16f16_t, T) = delete;
template <typename T> constexpr bool operator<(T, i16f16_t) = delete;
template <typename T> constexpr bool operator<=(i16f16_t, T) = delete;
template <typename T> constexpr bool operator<=(T, i16f16_t) = delete;
template <typename T> constexpr bool operator>(i16f16_t, T) = delete;
template <typename T> constexpr bool operator>(T, i16f16_t) = delete;
template <typename T> constexpr bool operator>=(i16f16_t, T) = delete;
template <typename T> constexpr bool operator>=(T, i16f16_t) = delete;

// Increment/Decrement

constexpr i16f16_t &operator++(i16f16_t &a) {
  a.value += 65536;
  return a;
}
constexpr i16f16_t operator++(i16f16_t &a, int) {
  i16f16_t tmp = a;
  a.value += 65536;
  return tmp;
}
constexpr i16f16_t &operator--(i16f16_t &a) {
  a.value -= 65536;
  return a;
}
constexpr i16f16_t operator--(i16f16_t &a, int) {
  i16f16_t tmp = a;
  a.value -= 65536;
  return tmp;
}

struct Vec2 {
  i16f16_t x, y;

  Vec2() = default;
  constexpr explicit Vec2(int _x, int _y) : x(_x), y(_y) {}
  constexpr explicit Vec2(i16f16_t _x, i16f16_t _y) : x(_x), y(_y) {}
};

constexpr Vec2 operator-(const Vec2 &a) { return Vec2(-a.x, -a.y); }
constexpr Vec2 operator+(const Vec2 &a) { return a; }

constexpr Vec2 operator+(const Vec2 &a, const Vec2 &b) { return Vec2(a.x + b.x, a.y + b.y); }
constexpr Vec2 operator-(const Vec2 &a, const Vec2 &b) { return Vec2(a.x - b.x, a.y - b.y); }
constexpr Vec2 operator*(const Vec2 &a, const Vec2 &b) { return Vec2(a.x * b.x, a.y * b.y); }
constexpr Vec2 operator/(const Vec2 &a, const Vec2 &b) { return Vec2(a.x / b.x, a.y / b.y); }
constexpr Vec2 operator*(const Vec2 &a, i16f16_t s) { return Vec2(a.x * s, a.y * s); }
constexpr Vec2 operator/(const Vec2 &a, i16f16_t s) { return Vec2(a.x / s, a.y / s); }
constexpr Vec2 &operator+=(Vec2 &a, const Vec2 &b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}
constexpr Vec2 &operator-=(Vec2 &a, const Vec2 &b) {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}
constexpr bool operator==(const Vec2 &a, const Vec2 &b) { return a.x == b.x && a.y == b.y; }
constexpr bool operator!=(const Vec2 &a, const Vec2 &b) { return a.x != b.x && a.y != b.y; }

struct Vec3 {
  i16f16_t x, y, z;

  Vec3() = default;
  constexpr explicit Vec3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
  constexpr explicit Vec3(i16f16_t _x, i16f16_t _y, i16f16_t _z) : x(_x), y(_y), z(_z) {}
};

constexpr Vec3 operator-(const Vec3 &a) { return Vec3(-a.x, -a.y, -a.z); }
constexpr Vec3 operator+(const Vec3 &a) { return a; }

constexpr Vec3 operator+(const Vec3 &a, const Vec3 &b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
constexpr Vec3 operator-(const Vec3 &a, const Vec3 &b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
constexpr Vec3 operator*(const Vec3 &a, const Vec3 &b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
constexpr Vec3 operator/(const Vec3 &a, const Vec3 &b) { return Vec3(a.x / b.x, a.y / b.y, a.z / b.z); }
constexpr Vec3 operator*(const Vec3 &a, i16f16_t s) { return Vec3(a.x * s, a.y * s, a.z * s); }
constexpr Vec3 operator/(const Vec3 &a, i16f16_t s) { return Vec3(a.x / s, a.y / s, a.z / s); }
constexpr Vec3 &operator+=(Vec3 &a, const Vec3 &b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}
constexpr Vec3 &operator-=(Vec3 &a, const Vec3 &b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}

struct Line2 {
  Vec2 v1, v2;

  Line2() = default;
  constexpr explicit Line2(Vec2 _v1, Vec2 _v2) : v1(_v1), v2(_v2) {}
};

struct Line3 {
  Vec3 v1, v2;

  Line3() = default;
  constexpr explicit Line3(Vec3 _v1, Vec3 _v2) : v1(_v1), v2(_v2) {}
};

struct Tri2 {
  Vec2 v1, v2, v3;

  Tri2() = default;
  constexpr explicit Tri2(Vec2 _v1, Vec2 _v2, Vec2 _v3) : v1(_v1), v2(_v2), v3(_v3) {}

  inline i16f16_t area() const { return (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x); }

  inline Vec2 center() const { return Vec2((v1.x + v2.x + v3.x) / i16f16_t(3), (v1.y + v2.y + v3.y) / i16f16_t(3)); }
};

struct Tri3 {
  Vec3 v1, v2, v3;

  Tri3() = default;
  constexpr explicit Tri3(Vec3 _v1, Vec3 _v2, Vec3 _v3) : v1(_v1), v2(_v2), v3(_v3) {}
};

struct Rect2 {
  Vec2 xy, wh;

  Rect2() = default;
  constexpr explicit Rect2(Vec2 _xy, Vec2 _wh) : xy(_xy), wh(_wh) {}
};

constexpr i16f16_t abs(i16f16_t a) { return (a.value < 0) ? i16f16_t(-a.value, true) : a; }
constexpr Vec2 abs(const Vec2 &v) { return Vec2(abs(v.x), abs(v.y)); }
constexpr Vec3 abs(const Vec3 &v) { return Vec3(abs(v.x), abs(v.y), abs(v.z)); }

// Random utilities
namespace Util {

enum class FmtType {
  NONE,
  INT8,
  UINT8,
  INT16,
  UINT16,
  INT32,
  UINT32,
  INT64,
  UINT64,
  CSTR,
  I16F16,
};

struct fmtArg {
  FmtType type;
  const void *ptr;
};

template <typename T, typename U> struct isSame {
  static constexpr bool value = false;
};

template <typename T> struct isSame<T, T> {
  static constexpr bool value = true;
};

template <typename T, typename U> constexpr bool isSameV = isSame<T, U>::value;

template <typename T> struct makeUnsigned;
template <> struct makeUnsigned<int8_t> {
  using type = uint8_t;
};
template <> struct makeUnsigned<uint8_t> {
  using type = uint8_t;
};
template <> struct makeUnsigned<int16_t> {
  using type = uint16_t;
};
template <> struct makeUnsigned<uint16_t> {
  using type = uint16_t;
};
template <> struct makeUnsigned<int32_t> {
  using type = uint32_t;
};
template <> struct makeUnsigned<uint32_t> {
  using type = uint32_t;
};
template <> struct makeUnsigned<int64_t> {
  using type = uint64_t;
};
template <> struct makeUnsigned<uint64_t> {
  using type = uint64_t;
};

template <typename T> using makeUnsignedT = typename makeUnsigned<T>::type;

template <typename T>
constexpr FmtType argTypeV = isSameV<T, int8_t>         ? FmtType::INT8
                             : isSameV<T, uint8_t>      ? FmtType::UINT8
                             : isSameV<T, int16_t>      ? FmtType::INT16
                             : isSameV<T, uint16_t>     ? FmtType::UINT16
                             : isSameV<T, int32_t>      ? FmtType::INT32
                             : isSameV<T, uint32_t>     ? FmtType::UINT32
                             : isSameV<T, int64_t>      ? FmtType::INT64
                             : isSameV<T, uint64_t>     ? FmtType::UINT64
                             : isSameV<T, const char *> ? FmtType::CSTR
                             : isSameV<T, i16f16_t>     ? FmtType::I16F16
                                                        : FmtType::NONE;

struct FmtStr {
  const char *data;
  size_t size;

  template <size_t N> constexpr FmtStr(const char (&s)[N]) : data(s), size(N - 1) {}
  operator const char *() const { return data; }
  char operator[](size_t i) const { return data[i]; }
};

template <typename T> constexpr bool isSignedV = T(-1) < T(0);

template <typename T, size_t N> struct Array {
  T arr[N];
  template <typename... Ts>
    requires(sizeof...(Ts) == N)
  constexpr Array(Ts &&...ts) : arr {ts...} {}
  constexpr Array() = default;
  constexpr size_t size() const { return N; }

  constexpr T &operator[](size_t i) { return arr[i]; }
  constexpr T const &operator[](size_t i) const { return arr[i]; }

  constexpr T *begin() { return arr; }
  constexpr T *end() { return arr + N; }
};

template <typename T> struct Vector {
  T *data = nullptr;
  size_t __capacity = 0;
  size_t __size = 0;

  Vector() = default;
  ~Vector() {
    if (data)
      MM::free(data);
  }

  Vector(const Vector &) = delete;
  Vector &operator=(const Vector &) = delete;

  Vector(Vector &&other) noexcept : data(other.data), __capacity(other.__capacity), __size(other.__size) {
    other.data = nullptr;
    other.__capacity = 0;
    other.__size = 0;
  }

  Vector &operator=(Vector &&other) noexcept {
    if (this == &other)
      return *this;

    if (data)
      MM::free(data);

    data = other.data;
    __capacity = other.__capacity;
    __size = other.__size;
    other.data = nullptr;
    other.__capacity = 0;
    other.__size = 0;

    return *this;
  }

  void reserve(size_t size) {
    if (size <= __size)
      return;

    T *newData;
    if (!data)
      newData = static_cast<T *>(MM::malloc(size * sizeof(T)));
    else
      newData = static_cast<T *>(MM::realloc(data, size * sizeof(T)));
    if (!newData)
      return;

    data = newData;
    __capacity = size;
  }

  void pushBack(const T &value) {
    if (__size == __capacity)
      reserve(__capacity == 0 ? 8 : __capacity * 2);

    data[__size++] = value;
  }

  void pushBack(T &&value) {
    if (__size == __capacity)
      reserve(__capacity == 0 ? 8 : __capacity * 2);

    data[__size++] = std::move(value);
  }

  void pushBack() {
    if (__size == __capacity)
      reserve(__capacity == 0 ? 8 : __capacity * 2);

    data[__size++] = T();
  }

  void popBack() {
    if (!empty())
      --__size;
  }

  T &back() { return data[__size - 1]; }
  T &front() { return data[0]; }

  T &operator[](size_t i) { return data[i]; }
  T const &operator[](size_t i) const { return data[i]; }

  T *begin() { return data; }
  T *end() { return data + __size; }

  T const *begin() const { return data; }
  T const *end() const { return data + __size; }

  size_t size() const { return __size; }
  size_t capacity() const { return __capacity; }
  bool empty() const { return __size == 0; }
  void clear() { __size = 0; }

  void shrinkToFit() {
    if (__size == __capacity || __size == 0) {
      return;
    }
    T *newData = static_cast<T *>(MM::realloc(data, __size * sizeof(T)));
    if (newData) {
      data = newData;
      __capacity = __size;
    }
  }
};

template <size_t N> struct circBuffer {
  static constexpr uint32_t MASK = N - 1;
  static_assert((N & MASK) == 0, "N must be power of two");

  volatile char buf[N];
  volatile uint32_t head = 0;
  volatile uint32_t tail = 0;

  inline bool full() const { return ((head + 1) & MASK) == tail; }
  inline bool empty() const { return head == tail; }
  inline uint32_t available() const { return (head - tail) & MASK; }
  inline uint32_t freeSpace() const { return (tail - head - 1) & MASK; }

  inline bool append(char c) {
    if (full())
      return false;

    buf[head] = c;
    head = (head + 1) & MASK;
    return true;
  }

  inline bool pop(char &c) {
    if (empty())
      return false;

    c = buf[tail];
    tail = (tail + 1) & MASK;
    return true;
  }

  inline size_t copyOut(char *dst, size_t len) {
    uint32_t avail = available();
    uint32_t toRead = (len > avail) ? avail : len;
    MM::copyFromCirc(dst, (const void *)buf, N, tail, toRead);
    tail = (tail + toRead) & MASK;
    return toRead;
  }

  inline size_t copyIn(const char *src, size_t len) {
    uint32_t free = freeSpace();
    uint32_t toWrite = (len > free) ? free : len;
    MM::copyToCirc((void *)buf, N, head, src, toWrite);
    head = (head + toWrite) & MASK;
    return toWrite;
  }
};
} // namespace Util