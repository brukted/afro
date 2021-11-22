/**
 * @brief Contains custom math data types
 *
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <cmath>

template <typename T>
struct Vec2 {
  T x;
  T y;
};

using IVec2 = Vec2<int>;
using FVec2 = Vec2<float>;

template <typename T>
struct Vec3 {
  T x;
  T y;
  T z;
};

using IVec3 = Vec3<int>;
using FVec3 = Vec3<float>;

template <typename T>
struct Vec4 {
  T x;
  T y;
  T z;
  T w;
};

using IVec4 = Vec4<int>;
using FVec4 = Vec4<float>;
