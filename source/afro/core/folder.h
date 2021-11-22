/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/function_graph.h"
#include "core/image_texture.h"
#include "core/material_graph.h"

namespace afro::core {
struct Folder {
  const UUID uid;
  std::string name;
  Folder* parent_folder = nullptr;
  std::vector<std::unique_ptr<Folder>> sub_folders;
  std::vector<MaterialGraph*> material_graphs;
  std::vector<FunctionGraph*> function_graphs;
  std::vector<ImageTexture*> images;

  Folder(UUID uid, std::string name);

  auto add_item(MaterialGraph* graph) noexcept -> void;
  auto add_item(FunctionGraph* graph) noexcept -> void;
  auto add_item(ImageTexture* texture) noexcept -> void;
  auto add_item(std::unique_ptr<Folder> sub_folder) noexcept -> void;
  auto remove_item(MaterialGraph* graph) -> void;
  auto remove_item(FunctionGraph* graph) -> void;
  auto remove_item(ImageTexture* texture) -> void;
  auto remove_item(Folder* sub_folder) -> void;
};
}  // namespace afro::core