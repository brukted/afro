/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <string>

#include "core/uuid.h"

namespace afro::core {

struct NodeUiInfo {
  float x_pos = 0.0F, y_pos = 0.0F;
  bool is_hidden = false;
};

struct CommentNode {
  UUID uuid;
  std::string comment{};
  NodeUiInfo ui_info;
};

}  // namespace afro::core
