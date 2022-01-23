/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "curve.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <vector>

#include "utils/asset.h"
#include "utils/math.h"

namespace afro::core::bezier {
using std::abs;
using std::pow;

auto BezierSpline::at(float x, bool clamp) -> float {
  const auto [left, right] = get_left_right(x);
  const auto t = get_t(x, left, right);
  return evaluate_at_t(left, right, t, clamp).y;
}

auto BezierSpline::sort() -> void {
  std::ranges::sort(points, [](auto &a, auto &b) { return a.pos.x < b.pos.x; });
}

auto BezierSpline::lut(int samples, bool clamp) -> std::vector<float> {
  auto lut = std::vector<float>();
  lut.reserve(samples);
  const auto step = 1.0F / (float)samples;
  auto x = 0.0F;
  sort();
  for (auto sample = 0; x <= 1.0F && sample < samples; ++sample) {
    x = step * static_cast<float>(sample);
    lut.push_back(at(x, clamp));
  }
  return lut;
}

auto BezierSpline::split_at(float x) -> void {
  ControlPoint *left_cp = nullptr;
  ControlPoint *right_cp = nullptr;
  get_left_right(x, &left_cp, &right_cp);
  // t parameter of the curve at x
  const auto t = get_t(x, *left_cp, *right_cp);
  const auto f_points = std::vector<FVec2>{left_cp->pos, left_cp->t2, right_cp->t1, right_cp->pos};

  auto left = std::vector<FVec2>{};
  auto right = std::vector<FVec2>{};
  std::function<void(std::vector<FVec2>, float)> curve_point = [&left, &right, &curve_point](std::vector<FVec2> points,
                                                                                             float t) -> void {
    if (points.size() == 1) {
      left.push_back(points[0]);
      right.push_back(points[0]);
    } else {
      auto new_points = std::vector<FVec2>(points.size() - 1);
      for (size_t i = 0; i < new_points.size(); ++i) {
        if (i == 0) {
          left.push_back(points[i]);
        }
        if (i == new_points.size() - 1) {
          right.push_back(points[i + 1]);
        }
        new_points[i] = points[i] * (1 - t) + points[i + 1] * t;
      }
      curve_point(new_points, t);
    }
  };
  curve_point(f_points, t);
  auto new_point = ControlPoint{};
  new_point.pos = left[3];
  new_point.t1 = left[2];
  new_point.t2 = right[2];
  points.push_back(new_point);
  sort();
}

auto BezierSpline::get_t(float x, const ControlPoint &left, const ControlPoint &right, float elision) const -> float {
  AF_ASSERT(x >= 0 && x <= 1)
  /*
  // P(t) = (1-t)^3 * P0 + 3(1-t)^2 * t * P1 + 3(1-t) * t^2 * P2 + t^3 * P3
  // x(t) = (1-t)^3 * a + 3(1-t)^2 * t * b + 3(1-t) * t^2 * c + t^3 * d
  // solve t such that (-a + 3b - 3c + d)t^3 + (3a - 6b + 3c)t^2 + (-3a + 3b)t + (a-x) = 0
  const auto a = left.pos.x;   // P0.x
  const auto b = left.t2.x;    // P1.x
  const auto c = right.t1.x;   // P2.x
  const auto d = right.pos.x;  // P3.x
  // Cubic terms
  const auto c_a = -a + (3 * b) - (3 * c) + d;
  const auto c_b = (3 * a) - (6 * b) + (3 * c);
  const auto c_c = (-3 * a) + (3 * b);
  const auto c_d = a - x;
  const auto roots = cubic_roots(c_a, c_b, c_c, c_d);
  for (const auto &root : roots) {
    if (root >= 0 && root <= 1) {
      return (float)root;
    }
  }
  return std::nan(""); */
  // We use binary search to find the t value. Using Cubic roots has proved to be finicky.
  constexpr auto half = 0.5F;
  float lower = 0.0F;
  float upper = 1.0F;
  float mid = half;
  auto p = evaluate_at_t(left, right, mid);

  while (abs(x - p.x) > elision) {
    AF_ASSERT(lower <= mid && mid <= upper)
    AF_ASSERT(upper - lower > elision)
    if (p.x > x) {
      upper = mid;
    } else {
      lower = mid;
    }
    mid = (lower + upper) / 2.0F;
    p = evaluate_at_t(left, right, mid);
  }

  return mid;
}

auto BezierSpline::get_left_right(float x, ControlPoint **left, ControlPoint **right) -> void {
  AF_ASSERT(x >= 0 && x <= 1)
  *left = &points[0];
  *right = &points[points.size() - 1];
  for (auto &point : points) {
    if (point.pos.x < x && point.pos.x > (*left)->pos.x) {
      *left = &point;
    }
    if (point.pos.x > x && point.pos.x < (*right)->pos.x) {
      *right = &point;
    }
  }
}

auto BezierSpline::get_left_right(float x) -> std::pair<ControlPoint, ControlPoint> {
  AF_ASSERT(x >= 0 && x <= 1)
  ControlPoint *left = nullptr;
  ControlPoint *right = nullptr;
  get_left_right(x, &left, &right);
  AF_ASSERT(left != nullptr && right != nullptr)
  return {*left, *right};
}

auto BezierSpline::evaluate_at_t(const ControlPoint &left, const ControlPoint &right, float t, bool clamp) -> FVec2 {
  AF_ASSERT(t >= 0 && t <= 1)
  // clang-format off
  // P(t) = (1-t)^3 * P0 + 3(1-t)^2 * t * P1 + 3(1-t) * t^2 * P2 + t^3 * P3
  auto p = left.pos * pow(1-t,3.0F) +
           left.t2 * 3.0F * pow(1-t,2.0F) * t + 
           right.t1 * 3.0F * (1-t) * pow(t,2.0F) +
           right.pos * pow(t,3.0F);
  // clang-format on
  if (clamp) {
    p.y = std::clamp(p.y, 0.0F, 1.0F);
    p.x = std::clamp(p.x, 0.0F, 1.0F);
  }
  return p;
}

auto BezierSpline::validate() -> void {
  AF_ASSERT(points.size() >= 2)
  auto &cp = points[0];
  cp.t1 = {0.0F, 0.0F};
  cp.pos.x = std::clamp(cp.pos.x, 0.0F, points[1].pos.x);
  cp.t2.x = std::clamp(cp.t2.x, cp.pos.x, points[1].pos.x);

  for (size_t i = 1; i < points.size() - 1; ++i) {
    auto &prev = points[i - 1];
    auto &cur = points[i];
    auto &next = points[i + 1];
    cur.pos.x = std::clamp(cur.pos.x, prev.pos.x, next.pos.x);
    cur.t1.x = std::clamp(cur.t1.x, prev.pos.x, cur.pos.x);
    cur.t2.x = std::clamp(cur.t2.x, cur.pos.x, next.pos.x);
  }

  auto &l_p = points.back();
  l_p.pos.x = std::clamp(l_p.pos.x, points[points.size() - 2].pos.x, 1.0F);
  l_p.t1.x = std::clamp(l_p.t1.x, points[points.size() - 2].pos.x, l_p.pos.x);
  l_p.t2 = {1.0F, 1.0F};
}
}  // namespace afro::core::bezier