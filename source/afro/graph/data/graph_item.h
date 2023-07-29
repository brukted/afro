/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "common/interfaces/object.h"

namespace afro::graph {

class GraphItem : public AfObject {
 public:
  IVec2 position = {0, 0};
  bool is_visible = true;
  ~GraphItem() override = default;
};

}  // namespace afro::graph