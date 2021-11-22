/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "folder.h"

#include <stdexcept>
#include <utility>

namespace afro::core {
Folder::Folder(UUID uid, std::string name) : uid(uid), name(std::move(name)) {}

auto Folder::add_item(MaterialGraph* graph) noexcept -> void { material_graphs.push_back(graph); }

auto Folder::add_item(FunctionGraph* graph) noexcept -> void { function_graphs.push_back(graph); }

auto Folder::add_item(ImageTexture* texture) noexcept -> void { images.push_back(texture); }

auto Folder::add_item(std::unique_ptr<Folder> sub_folder) noexcept -> void {
  sub_folder->parent_folder = this;
  sub_folders.emplace_back(std::move(sub_folder));
}

auto Folder::remove_item(MaterialGraph* graph) -> void {
  auto pos = std::remove_if(std::begin(material_graphs), std::end(material_graphs),
                            [&](auto* val) { return graph->uuid == val->uuid; });
  if (pos != std::end(material_graphs)) {
    material_graphs.erase(pos);
  } else {
    throw std::runtime_error("Graph doesn't exist in the folder");
  }
}

auto Folder::remove_item(FunctionGraph* graph) -> void {
  auto pos = std::remove_if(std::begin(function_graphs), std::end(function_graphs),
                            [&](auto* val) { return graph->uid == val->uid; });
  if (pos != std::end(function_graphs)) {
    function_graphs.erase(pos);
  } else {
    throw std::runtime_error("Graph doesn't exist in the folder");
  }
}

auto Folder::remove_item(ImageTexture* texture) -> void {
  auto pos = std::remove_if(std::begin(images), std::end(images),
                            [&](auto* val) { return texture->uid == val->uid; });
  if (pos != std::end(images)) {
    images.erase(pos);
  } else {
    throw std::runtime_error("Image doesn't exist in the folder");
  }
}

auto Folder::remove_item(Folder* sub_folder) -> void {
  auto pos = std::remove_if(std::begin(sub_folders), std::end(sub_folders),
                            [&](auto& val) { return sub_folder->uid == val->uid; });
  if (pos != std::end(sub_folders)) {
    sub_folders.erase(pos);
  } else {
    throw std::runtime_error("Folder doesn't exist in the folder");
  }
}
}  // namespace afro::core