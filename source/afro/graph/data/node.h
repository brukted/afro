/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <boost/signals2/signal.hpp>
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
  boost::signals2::signal<void()> on_invalidate;

  Node(UUID uuid, std::vector<property::Property> properties, std::string name)
      : AfObject(uuid, std::move(properties)), name(std::move(name)) {
    position = {0, 0};
    for (auto& prop : this->get_properties()) {
      if (prop.get_property_definition().type == property::Type::INPUT) {
        prop.on_value_changed.connect([this](auto&) {
          log::core_trace("Node::on_invalidate");
          on_invalidate();
        });
      }
    }
  }

  Node(std::vector<property::Property> properties, std::string name)
      : AfObject(std::move(properties)), name(std::move(name)) {
    position = {0, 0};
    for (auto& prop : this->get_properties()) {
      if (prop.get_property_definition().type == property::Type::INPUT) {
        prop.on_value_changed.connect([this](auto&) {
          log::core_trace("Node::on_invalidate");
          on_invalidate();
        });
      }
    }
  }

  auto get_name() -> std::string_view { return name; }

  ~Node() override = default;
};

}  // namespace afro::graph