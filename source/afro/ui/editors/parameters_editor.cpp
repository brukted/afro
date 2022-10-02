/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "parameters_editor.h"

#include "core/folder.h"
#include "core/function_graph.h"
#include "core/image_texture.h"
#include "core/material_graph.h"
#include "imgui.h"
#include "utils/translation.h"

using namespace ImGui;

namespace afro::ui {
auto ParameterEditor::draw(bool* p_open) -> void {
  if (!Begin(translate("Properties Editor"), p_open)) {
    End();
    return;
  }
  if (mat_node != nullptr) {
    draw_material_node();
  } else if (fun_node != nullptr) {
    fun_node->props.draw();
  } else if (mat_graph != nullptr) {
    mat_graph->props.draw();
  } else if (folder != nullptr) {
  } else if (img_texture != nullptr) {
  } else if (fun_graph != nullptr) {
  }
  End();
}

auto ParameterEditor::unbind() -> void {
  mat_node = nullptr;
  fun_node = nullptr;
  fun_graph = nullptr;
  mat_graph = nullptr;
  folder = nullptr;
  img_texture = nullptr;
}

auto ParameterEditor::open(core::MaterialNode* node) -> void {
  unbind();
  mat_node = node;
}

auto ParameterEditor::open(core::FunctionNode* node) -> void {
  unbind();
  fun_node = node;
}

auto ParameterEditor::open(core::FunctionGraph* graph) -> void {
  unbind();
  fun_graph = graph;
}

auto ParameterEditor::open(core::MaterialGraph* graph) -> void {
  unbind();
  mat_graph = graph;
}

auto ParameterEditor::open(core::Folder* folder) -> void {
  unbind();
  this->folder = folder;
}

auto ParameterEditor::open(core::ImageTexture* image) -> void {
  unbind();
  img_texture = image;
}
auto ParameterEditor::draw_material_node() -> void {
  if (CollapsingHeader(translate("Common Properties"))) {
    mat_node->common_props.draw();
  }
  if (ImGui::CollapsingHeader(translate("Specfic Properties"))) {
    mat_node->common_props.draw();
  }
}
}  // namespace afro::ui
