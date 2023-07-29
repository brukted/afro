/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <string_view>
#include <vector>

#include "common/data/uuid.h"
#include "common/interfaces/object.h"
#include "property/data/property.h"

namespace afro::graph {

class Node : public AfObject {
 private:
  std::string name = "Node";

 public:
  FVec2 position = {0, 0};
  bool is_visible = true;

  Node(UUID uuid, std::vector<property::Property> properties, std::string name)
      : AfObject(uuid, std::move(properties)), name(std::move(name)) {
    position = {0, 0};
  }

  Node(std::vector<property::Property> properties, std::string name)
      : AfObject(std::move(properties)), name(std::move(name)) {}

  auto get_name() -> std::string_view { return name; }

  ~Node() override = default;
};

}  // namespace afro::graph