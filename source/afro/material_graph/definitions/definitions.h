/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <fruit/fruit.h>

#include "material_graph/data/material_node_definition.h"

namespace afro::graph::material {
class NodeDefinitions : public std::vector<MaterialNodeDefinition> {
 private:
  static auto get_node_definitions() -> std::vector<MaterialNodeDefinition>;

 public:
  INJECT(NodeDefinitions())
      : std::vector<MaterialNodeDefinition>(get_node_definitions()){};
};
}  // namespace afro::graph::material
