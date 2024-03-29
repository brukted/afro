/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_editor.h"

#include <imgui.h>

#include "graph/commands/add_node_command.h"
#include "imnodes/imnodes.h"
#include "ui/utils/ui_utils.h"
#include "utils/translation.h"

namespace afro::graph::material {
auto MaterialEditor::draw() -> void {
  // Draw the main menu bar
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(translate("Editors"))) {
      ImGui::MenuItem(translate("Material Editor"), nullptr, &show);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (graph != nullptr) {
    engine->update();
  }

  GraphEditor::draw();
}

auto MaterialEditor::draw_node_body(Node& node) -> void {
  uintptr_t ptr =
      engine->get_preview_texture(dynamic_cast<MaterialNode&>(node));
  ImGui::Image(reinterpret_cast<ImTextureID>(ptr),
               {ImGui::GetFontSize() * 7, ImGui::GetFontSize() * 7});
}

auto MaterialEditor::draw_main_context_menu() -> void {
  // const auto mouse_pos = ImGui::GetMousePos();
  int hovered_node = 0;
  if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) &&
      ImGui::IsMouseDown(ImGuiMouseButton_Right) &&
      !ImNodes::IsNodeHovered(&hovered_node)) {
    ImGui::OpenPopup("main_context_menu");
  }
  if (ImGui::BeginPopup("main_context_menu")) {
    if (ImGui::BeginMenu(translate("Add node"))) {
      for (const auto& node_def : *node_definitions) {
        ui::draw_command<graph::AddNode>(undo_stack.get(), node_def.get_name(),
                                         node_def.get_icon(), node_def, graph);
      }
      ImGui::EndMenu();
    }
    ImGui::EndPopup();
  }
}

auto MaterialEditor::set_graph(const std::shared_ptr<MaterialGraph> graph)
    -> void {
  GraphEditor::set_graph(graph);
  engine->set_graph(graph);

  // Listen for node change
  for (const auto& node : graph->get_nodes()) {
    connections.push_back(node->on_invalidate.connect([this, node]() {
      log::core_trace("Node changed: {}", node->get_uuid());
      engine->on_node_changed(node->get_uuid());
    }));
  }

  graph->node_added.connect([this](const std::shared_ptr<Node>& node) {
    log::core_trace("Node added: {}", node->get_name());
    engine->on_node_created(std::dynamic_pointer_cast<MaterialNode>(node));
    node->on_invalidate.connect([this, node]() {
      log::core_trace("Node changed: {}", node->get_uuid());
      engine->on_node_changed(node->get_uuid());
    });
  });

  // Listen for node removal
  graph->node_removed.connect([this](const std::shared_ptr<Node>& node) {
    node->on_invalidate.disconnect_all_slots();
    engine->on_node_deleted(std::dynamic_pointer_cast<MaterialNode>(node));
    log::core_trace("Node removed: {}", node->get_name());
  });

  // Listen for link change
  graph->link_added.connect([this](Link link) {
    log::core_trace("Link created: {} -> {}", link.get_from_property(),
                    link.get_to_property());
    engine->on_link_created(link);
  });
  graph->link_removed.connect([this](Link link) {
    log::core_trace("Link removed: {} -> {}", link.get_from_property(),
                    link.get_to_property());
    engine->on_link_deleted(link);
  });
}

auto MaterialEditor::clear_graph() -> void {
  GraphEditor::clear_graph();
  engine->clear_graph();
  graph->node_added.disconnect_all_slots();
  graph->node_removed.disconnect_all_slots();
  graph->link_added.disconnect_all_slots();
  graph->link_removed.disconnect_all_slots();
}
auto MaterialEditor::shutdown() -> void { engine->shutdown(); }
}  // namespace afro::graph::material