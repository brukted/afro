/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <glbinding/gl/enum.h>

#include <utility>
#include <vector>

#include "common/data/uuid.h"
#include "graph/data/link.h"
#include "graph/data/node.h"
#include "material_node_definition.h"
#include "property/data/property.h"

namespace afro::graph::material {
class MaterialNode : public Node {
 private:
  MaterialNodeDefinition definition;

 public:
  MaterialNode(UUID uuid, std::vector<property::Property> properties,
               const MaterialNodeDefinition& definition)
      : Node(uuid, std::move(properties), definition.get_name()),
        definition(definition) {}

  MaterialNode(std::vector<property::Property> properties,
               const MaterialNodeDefinition& definition)
      : Node(std::move(properties), definition.get_name()),
        definition(definition) {}

  static auto create(const MaterialNodeDefinition& definition)
      -> std::shared_ptr<MaterialNode> {
    auto properties = std::vector<property::Property>();
    for (const auto& prop_def : definition.get_prop_definitions()) {
      properties.emplace_back(prop_def);
    }
    return std::make_shared<MaterialNode>(std::move(properties), definition);
  }

  static auto create(const MaterialNodeDefinition& definition, UUID uuid)
      -> std::shared_ptr<MaterialNode> {
    auto properties = std::vector<property::Property>();
    for (const auto& prop_def : definition.get_prop_definitions()) {
      properties.emplace_back(prop_def);
    }
    return std::make_shared<MaterialNode>(uuid, std::move(properties),
                                          definition);
  }

  [[nodiscard]] auto get_definition() -> auto& { return definition; }

  auto get_buffer_size() const -> IVec2;
  auto get_buffer_format() const -> gl::GLenum;
  auto get_property(std::string_view prop_id) -> property::Property&;
};

}  // namespace afro::graph::material
