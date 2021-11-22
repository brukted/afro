/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>
#include <vector>

#include "folder.h"
#include "function_graph.h"
#include "image_texture.h"
#include "material_graph.h"

namespace afro::core {

struct Data {
  std::vector<std::unique_ptr<MaterialGraph>> material_graphs;
  std::vector<std::unique_ptr<FunctionGraph>> function_graphs;
  std::vector<std::unique_ptr<ImageTexture>> image_textures;
  Folder root_folder{0, "root"};
};

}  // namespace afro::core
