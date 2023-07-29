/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <vector>

#include "common/data/uuid.h"
#include "common/interfaces/object.h"
#include "property/data/property.h"

namespace afro::graph {

class Node : public AfObject {
 public:
  IVec2 position = {0, 0};
  bool is_visible = true;

  Node(UUID uuid, std::vector<property::Property> properties)
      : AfObject(uuid, std::move(properties)) {}

  Node(std::vector<property::Property> properties)
      : AfObject(std::move(properties)) {}

  ~Node() override = default;
};

}  // namespace afro::graph