/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "utils/math.h"

#include <cmath>
#include <numbers>

namespace afro {
auto cubic_roots(double a, double b, double c, double d) -> std::array<double, 3> {
  constexpr auto pi2 = std::numbers::pi * 2;
  auto qr = [](double a2, double a1, double a0, double& q, double& r) -> void {
    q = (3 * a1 - a2 * a2) / 9.0;
    r = (9.0 * a2 * a1 - 27 * a0 - 2 * a2 * a2 * a2) / 54.0;
  };
  auto pow_third = [](double x) -> double { return std::pow(std::abs(x), 1.0 / 3.0) * std::copysign(x, x); };
  d /= a;
  c /= a;
  b /= a;
  a = 1;
  double q = 0;
  double r = 0;
  qr(b, c, d, q, r);

  const auto q3 = q * q * q;
  const auto D = q3 + r * r;
  const auto shift = -b / 3;

  double x1 = 0;
  double x2 = nan("");
  double x3 = nan("");

  if (D >= 0) {
    const double sqrt_d = std::sqrt(D);
    const double s = pow_third(r + sqrt_d);
    const double t = pow_third(r - sqrt_d);

    x1 = shift + s + t;
    if (D == 0) {
      x2 = shift - s;
    }
  } else {
    const double theta = std::acos(r / std::sqrt(-q3));
    x1 = 2 * std::sqrt(-q) * std::cos(theta / 3) + shift;
    x2 = 2 * std::sqrt(-q) * std::cos((theta + pi2) / 3) + shift;
    x3 = 2 * std::sqrt(-q) * std::cos((theta - pi2) / 3) + shift;
  }
  return {x1, x2, x3};
}

}  // namespace afro