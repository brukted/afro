/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <vector>

namespace afro::core {

struct PackedFile {
  std::vector<uint8_t> bytes;
};

}  // namespace afro::core