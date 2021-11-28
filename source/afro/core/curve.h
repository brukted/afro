/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <vector>

#include "utils/math.h"

namespace afro::core::bezier {
/**
 * @brief Represents a single point on the curve editor.
 *
 */
struct ControlPoint {
  /**
   * @brief P3 of the pervious point.
   *
   */
  FVec2 t1;
  /**
   * @brief P4 of the pervious point and P1 of this curve.
   *
   */
  FVec2 pos;
  /**
   * @brief P2 of this curve.
   *
   */
  FVec2 t2;
};

/**
 * @brief A cubic bezier curve.
 *
 */
struct BezierSpline {
  static constexpr float DEFAULT_ELISION = 1e-5F;
  std::vector<ControlPoint> points = {{{0, 0}, {0, 0}, {0, 0}}, {{1, 1}, {1, 1}, {1, 1}}};
  /**
   * @brief Computes the y value of the curve at the given x value.
   *
   * @param x x coordinate of the point.
   * @param clamp whether to clamp the values to the range [0, 1].
   * @return float the y value of the curve at the given x value.
   * @warning Ensure the curve is sorted before calling this function.
   * @see https://pomax.github.io/BezierInfo-2/#yforx
   */
  [[nodiscard]] auto at(float x, bool clamp = true) -> float;
  /**
   * @brief Sort the control points in ascending order of x.
   *
   */
  auto sort() -> void;
  /**
   * @brief Builds a lookup tables for the curve.
   *
   * @param samples How many samples to use.
   * @param clamp Whether to clamp the values to the range [0, 1].
   * @return std::vector<float> the lookup table.
   */
  [[nodiscard]] auto lut(int samples, bool clamp = true) -> std::vector<float>;
  /**
   * @brief Inserts a control point at the given x value so as the curve's shape is not affected.
   *
   * @param x x coordinate to insert the new control point.
   * @see https://pomax.github.io/BezierInfo-2/#splitting
   */
  auto split_at(float x) -> void;
  /**
   * @brief Makes the curve has no intersecting segments.
   * @warning Make sure to call sort before calling this.
   */
  auto validate() -> void;

 private:
  /**
   * @brief Get the t parameter of the curve at the given x value.
   *
   * @param x x coordinate of the point.
   * @param left the left control point.
   * @param right the right control point.
   * @return float the t parameter of the curve at the given x value.
   */
  [[nodiscard]] auto get_t(float x, const ControlPoint& left, const ControlPoint& right,
                           float elision = DEFAULT_ELISION) const -> float;
  /**
   * @brief Get the left and right control points of the curve at the given x value.
   *
   * @param x x coordinate of the point.
   * @param left out the left control point.
   * @param right out the right control point.
   */
  auto get_left_right(float x, ControlPoint** left, ControlPoint** right) -> void;
  /**
   * @brief Get the left and right control points of the curve at the given x value.
   *
   * @param x x coordinate of the point.
   */
  auto get_left_right(float x) -> std::pair<ControlPoint, ControlPoint>;
  [[nodiscard]] static auto evaluate_at_t(const ControlPoint& left, const ControlPoint& right, float t,
                                          bool clamp = true) -> FVec2;
};

}  // namespace afro::core::bezier
