/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <fruit/fruit.h>

#include <memory>
#include <vector>

#include "folder.h"
#include "material_graph/data/material_graph.h"

namespace afro::store {

class Data {
 public:
  INJECT(Data()) {}
  std::vector<std::unique_ptr<graph::material::MaterialGraph>> material_graphs;
  Folder root_folder{0, "root"};
};

}  // namespace afro::store
