#pragma once
#include <cstdint>
/*
 * Tämä tiedosto on tuotu omasta projektistani
 * https://github.com/ItsDuska/CelestiaWorks/blob/2D-Engine/CelestiaWorks/include/CelestiaWorks/System/Vector.hpp
 */

namespace life {

template <int L, typename T> struct Vec;
template <typename T> struct Vec<2, T> {
  T x, y;

  // <Constructor>
  // constexpr Vec() = default;
  // constexpr Vec(Vec const &v) = default;

  constexpr Vec() : x(0), y(0) {}
  constexpr Vec(Vec<2, T> const &value) : x(value.x), y(value.y) {}
  constexpr explicit Vec(T value) : x(value), y(value) {}
  constexpr Vec(T _x, T _y) : x(_x), y(_y) {}

  template <typename U>
  constexpr Vec(U _x, U _y) : x(static_cast<T>(_x)), y(static_cast<T>(_y)) {}

  //  <Artihmetic operators>
  constexpr Vec<2, T> &operator=(Vec const &v) = default;

  template <typename U> constexpr Vec<2, T> &operator=(Vec<2, U> const &v) {
    this->x = static_cast<T>(v.x);
    this->y = static_cast<T>(v.y);
    return *this;
  }

  template <typename U> constexpr Vec<2, T> &operator+=(Vec<2, U> const &v) {
    x += static_cast<T>(v.x);
    y += static_cast<T>(v.y);
    return *this;
  }

  template <typename U> constexpr Vec<2, T> &operator-=(Vec<2, U> const &v) {
    this->x -= static_cast<T>(v.x);
    this->y -= static_cast<T>(v.y);
    return *this;
  }

  template <typename U> constexpr Vec<2, T> &operator*=(Vec<2, U> const &v) {
    this->x *= static_cast<T>(v.x);
    this->y *= static_cast<T>(v.y);
    return *this;
  }

  template <typename U> constexpr Vec<2, T> &operator/=(Vec<2, U> const &v) {
    this->x /= static_cast<T>(v.x);
    this->y /= static_cast<T>(v.y);
    return *this;
  }
};
typedef Vec<2, float> Vec2;
typedef Vec<2, int32_t> Vec2i32;
typedef Vec<2, uint32_t> Vec2u32;
typedef Vec<2, uint16_t> Vec2u16;
typedef Vec<2, int16_t> Vec2i16;

} // namespace life
