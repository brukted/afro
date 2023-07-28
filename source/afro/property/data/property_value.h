/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <string>
#include <variant>

#include "enum_item.h"
#include "utils/math.h"

namespace afro::property {
enum class Type { INPUT, OUTPUT, DESCRIPTOR };
enum class ValueType {
  INTEGER,
  INTEGER_2,
  INTEGER_3,
  INTEGER_4,
  FLOAT,
  FLOAT_2,
  FLOAT_3,
  FLOAT_4,
  STRING,
  BOOLEAN,
  ENUM,
  COLOR_BEZIER_CURVE
};
enum class ValueUnit { NONE, ROTATION, POSITION, POWER_2, COLOR, PATH };

using PropertyValue = std::variant<int, IVec2, IVec3, IVec4, float, FVec2,
                                   FVec3, FVec4, std::string, bool, EnumItem>;
using OptPropertyValue = std::optional<PropertyValue>;
}  // namespace afro::property