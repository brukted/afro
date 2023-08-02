/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "graph/data/graph.h"
#include "material_node.h"

namespace afro::graph::material {

class MaterialGraph : public Graph {
 public:
  MaterialGraph() = default;
  ~MaterialGraph() override = default;
};

}  // namespace afro::graph::material
