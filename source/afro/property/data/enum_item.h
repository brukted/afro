/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <string>
#include <utility>

namespace afro::property {

struct EnumItem {
  const std::string name;
  const int value;

  EnumItem(std::string name, int value) : name(std::move(name)), value(value) {}
};

}  // namespace afro::property