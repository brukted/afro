/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "outliner.h"

#include <imgui.h>

#include <functional>
#include <memory>

#include "core/context.h"
#include "ui/icons.h"
#include "ui/ui_utils.h"
#include "undo/folder_ops.h"
#include "undo/material_graph_ops.h"
#include "utils/translation.h"

using namespace afro::core;

namespace afro::ui {
Outliner::Outliner(core::Context *context) : context(context) {}

auto Outliner::draw(bool *p_open) -> void {
  if (!ImGui::Begin(translate("Outliner"), p_open)) {
    ImGui::End();
    return;
  }
  main_context_menu();
  std::function<void(Folder &)> draw_folder = [&](Folder &folder) -> void {
    if (ImGui::TreeNode((void *)&folder, "%s %s", icon_code_point(Icon::FOLDER),
                        folder.name.c_str())) {
      for (auto &sub_folder : folder.sub_folders) {
        draw_folder(*sub_folder);
      }
      for (auto *mat_graph : folder.material_graphs) {
        ImGui::Text("%s %s", icon_code_point(Icon::MATERIAL_GRAPH), mat_graph->identifier.c_str());
        material_graph_menu(mat_graph);
      }
      for (auto *fun_graph : folder.function_graphs) {
        ImGui::TextUnformatted(fun_graph->identifier.c_str());
      }
      for (auto *image : folder.images) {
        ImGui::TextUnformatted(image->identifier.c_str());
      }
      ImGui::TreePop();
    }
  };
  draw_folder(context->data.root_folder);
  ImGui::End();
}

auto Outliner::main_context_menu() -> void {
  if (ImGui::BeginPopupContextWindow()) {
    if (ImGui::BeginMenu(translate("New"))) {
      draw_operator<AddNewMaterialGraph>(translate("Material Graph"), context, context,
                                         &context->data.root_folder);
      draw_operator<AddFolder>(translate("Folder"), context, &context->data.root_folder);
      // draw_operator<AddFunctionGraph>(translate("Function Graph"),context,
      // Args args...)
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(translate("Import"))) {
      // draw_operator<ImportImageTexture>(translate("Image
      // Texture"),context,Args args...)
      ImGui::EndMenu();
    }
    ImGui::Separator();
    // draw_operator<Paste>(translate("Paste"),context,Args args...)
    ImGui::EndPopup();
  }
}

auto Outliner::material_graph_menu(core::MaterialGraph *graph) -> void {
  if (ImGui::BeginPopupContextItem("###material_graph_popup")) {
    if (draw_operator<OpenMaterialGraph>(translate("Open"), context, context, graph)) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

}  // namespace afro::ui