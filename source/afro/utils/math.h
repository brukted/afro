/**
 * @brief Contains custom math data types
 *
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace afro {
template <typename T>
struct Vec2 {
  T x;
  T y;

  Vec2(T x = 0, T y = 0) : x(x), y(y) {}

  auto data() -> T* { return reinterpret_cast<T*>(this); }

  auto operator+(const Vec2<T>& b) const -> Vec2<T> {
    Vec2<T> vec;
    vec.x = x + b.x;
    vec.y = y + b.y;
    return vec;
  }
  auto operator-(const Vec2<T>& b) const -> Vec2<T> {
    Vec2<T> vec;
    vec.x = x - b.x;
    vec.y = y - b.y;
    return vec;
  }
  auto operator*(const T& b) const -> Vec2<T> {
    Vec2<T> vec;
    vec.x = x * b;
    vec.y = y * b;
    return vec;
  }
  auto operator/(const T& b) const -> Vec2<T> {
    Vec2<T> vec;
    vec.x = x / b;
    vec.y = y / b;
    return vec;
  }
};

using IVec2 = Vec2<int>;
using FVec2 = Vec2<float>;

template <typename T>
struct Vec3 {
  T x;
  T y;
  T z;

  Vec3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

  auto data() -> T* { return reinterpret_cast<T*>(this); }

  auto operator+(const Vec3<T>& b) const -> Vec3<T> {
    Vec3<T> vec;
    vec.x = x + b.x;
    vec.y = y + b.y;
    vec.z = z + b.z;
    return vec;
  }
  auto operator-(const Vec3<T>& b) const -> Vec3<T> {
    Vec3<T> vec;
    vec.x = x - b.x;
    vec.y = y - b.y;
    vec.z = z - b.z;
    return vec;
  }
  auto operator*(const T& b) const -> Vec3<T> {
    Vec3<T> vec;
    vec.x = x * b;
    vec.y = y * b;
    vec.z = z * b;
    return vec;
  }
  auto operator/(const T& b) const -> Vec3<T> {
    Vec3<T> vec;
    vec.x = x / b;
    vec.y = y / b;
    vec.z = z / b;
    return vec;
  }
};

using IVec3 = Vec3<int>;
using FVec3 = Vec3<float>;

template <typename T>
struct Vec4 {
  T x;
  T y;
  T z;
  T w;

  Vec4(T x = 0, T y = 0, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {}

  auto data() -> T* { return reinterpret_cast<T*>(this); }

  auto operator+(const Vec4<T>& b) const -> Vec4<T> {
    Vec4<T> vec;
    vec.x = x + b.x;
    vec.y = y + b.y;
    vec.z = z + b.z;
    vec.w = w + b.w;
    return vec;
  }
  auto operator-(const Vec4<T>& b) const -> Vec4<T> {
    Vec4<T> vec;
    vec.x = x - b.x;
    vec.y = y - b.y;
    vec.z = z - b.z;
    vec.w = w - b.w;
    return vec;
  }
  auto operator*(const T& b) const -> Vec4<T> {
    Vec4<T> vec;
    vec.x = x * b;
    vec.y = y * b;
    vec.z = z * b;
    vec.w = w * b;
    return vec;
  }
  auto operator/(const T& b) const -> Vec4<T> {
    Vec4<T> vec;
    vec.x = x / b;
    vec.y = y / b;
    vec.z = z / b;
    vec.w = w / b;
    return vec;
  }
};

template <typename T>
struct Range {
  T min;
  T max;

  Range(T min, T max) : min(min), max(max) {}
  auto clamp(T& val) -> void {
    val = static_cast<T>(std::clamp(val, min, max));
  };
};

using RangeF = Range<float>;
using RangeI = Range<int>;

using IVec4 = Vec4<int>;
using FVec4 = Vec4<float>;

/**
 * @brief Find all three complex roots of the cubic equation ax^3 + bx^2 + cx +
 * d=0
 *
 * @param a ax^3
 * @param b bx^2
 * @param c cx
 * @param d d
 * @return std::array<double, 3> the three roots if they exist, otherwise an NaN
 */
auto cubic_roots(double a, double b, double c, double d)
    -> std::array<double, 3>;
}  // namespace afro