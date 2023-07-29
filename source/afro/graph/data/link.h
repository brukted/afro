/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <string_view>
#include <tuple>

#include "common/data/uuid.h"

namespace afro::graph {

struct Link {
 private:
  UUID uuid;
  UUID from_node;
  UUID from_property;
  UUID to_node;
  UUID to_property;

 public:
  Link(UUID uuid, UUID to_node, UUID from_node, UUID to_property,
       UUID from_property)
      : uuid(uuid),
        to_node(to_node),
        from_node(from_node),
        to_property(to_property),
        from_property(from_property) {}

  Link(std::tuple<UUID, UUID> from, std::tuple<UUID, UUID> to)
      : uuid(generate_uuid()),
        from_node(std::get<0>(from)),
        from_property(std::get<1>(from)),
        to_node(std::get<0>(to)),
        to_property(std::get<1>(to)) {}

  auto operator==(const Link& rhs) const -> bool { return uuid == rhs.uuid; }
  auto operator==(const UUID& rhs) const -> bool { return uuid == rhs; }
  auto operator!=(const Link& rhs) const -> bool { return !(rhs == *this); }

  [[nodiscard]] auto get_uuid() const -> UUID { return uuid; }
  [[nodiscard]] auto get_to_node() const -> UUID { return to_node; }
  [[nodiscard]] auto get_from_node() const -> UUID { return from_node; }
  [[nodiscard]] auto get_to_property() const -> UUID { return to_property; }
  [[nodiscard]] auto get_from_property() const -> UUID { return from_property; }
};

}  // namespace afro::graph
