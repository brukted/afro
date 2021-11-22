/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

namespace afro::core {
struct MaterialNode;
struct FunctionNode;
struct Folder;
struct MaterialGraph;
struct FunctionGraph;
struct ImageTexture;
}  // namespace afro::core

namespace afro::ui {

struct ParameterEditor {
  core::MaterialNode* mat_node = nullptr;
  core::FunctionNode* fun_node = nullptr;
  core::MaterialGraph* mat_graph = nullptr;
  core::FunctionGraph* fun_graph = nullptr;
  core::Folder* folder = nullptr;
  core::ImageTexture* img_texture = nullptr;
  auto draw(bool* p_open) -> void;
  auto unbind() -> void;
  auto open(core::MaterialNode* node) -> void;
  auto open(core::FunctionNode* node) -> void;
  auto open(core::FunctionGraph* graph) -> void;
  auto open(core::MaterialGraph* graph) -> void;
  auto open(core::Folder* folder) -> void;
  auto open(core::ImageTexture* image) -> void;

 private:
  auto draw_material_node()->void;
};

}  // namespace afro::ui