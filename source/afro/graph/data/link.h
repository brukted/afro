/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <string_view>

#include "common/data/uuid.h"

namespace afro::graph {

struct Link {
 private:
  UUID uuid;
  UUID to_node;
  UUID from_node;
  std::string_view to_property;
  std::string_view from_property;

 public:
  Link(UUID uuid, UUID to_node, UUID from_node, std::string_view to_property,
       std::string_view from_property)
      : uuid(uuid),
        to_node(to_node),
        from_node(from_node),
        to_property(to_property),
        from_property(from_property) {}

  auto operator==(const Link& rhs) const -> bool { return uuid == rhs.uuid; }
  auto operator==(const UUID& rhs) const -> bool { return uuid == rhs; }
  auto operator!=(const Link& rhs) const -> bool { return !(rhs == *this); }

  [[nodiscard]] auto get_uuid() const -> UUID { return uuid; }
  [[nodiscard]] auto get_to_node() const -> UUID { return to_node; }
  [[nodiscard]] auto get_from_node() const -> UUID { return from_node; }
  [[nodiscard]] auto get_to_property() const -> std::string_view {
    return to_property;
  }
  [[nodiscard]] auto get_from_property() const -> std::string_view {
    return from_property;
  }
};

}  // namespace afro::graph
