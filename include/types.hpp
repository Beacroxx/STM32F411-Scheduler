#pragma once

#include <cmath>
#include <cstdint>

struct i16f16_t {
  int32_t value;

  // Constructors

  i16f16_t() = default;
  constexpr explicit i16f16_t(int32_t v) : value(static_cast<int32_t>(static_cast<int64_t>(v) * 65536)) {}
  constexpr explicit i16f16_t(const float v) : value(static_cast<int32_t>(v * 65536.0f)) {}
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

// Mixed-type arithmetic (integral types to f15_16_t)

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
  ++a.value;
  return a;
}
constexpr i16f16_t operator++(i16f16_t &a, int) {
  i16f16_t tmp = a;
  ++a.value;
  return tmp;
}
constexpr i16f16_t &operator--(i16f16_t &a) {
  --a.value;
  return a;
}
constexpr i16f16_t operator--(i16f16_t &a, int) {
  i16f16_t tmp = a;
  --a.value;
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

  inline i16f16_t area() { return (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x); }
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
