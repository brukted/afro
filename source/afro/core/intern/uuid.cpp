/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/uuid.h"

#include <random>

namespace afro::core {

auto generate_uuid() -> UUID {
  static std::random_device device;
  static std::mt19937_64 engine(device());
  static std::uniform_int_distribution<uint64_t> dist;
  return dist(engine);
}

}  // namespace afro::core