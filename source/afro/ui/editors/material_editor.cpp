/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_editor.h"

#include <GLFW/glfw3.h>
#include <glbinding/gl43core/gl.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "imnodes/imnodes.h"

#include <cstddef>
#include <memory>

#include "core/context.h"
#include "core/material_graph.h"
#include "core/material_nodes.h"
#include "ui/icons.h"
#include "ui/ui_utils.h"
#include "undo/material_graph_ops.h"
#include "utils/log.h"
#include "utils/translation.h"

using namespace afro::core;

namespace afro::ui {
MaterialEditor::MaterialEditor(core::Context *af_context)
    : af_context(af_context) {}

auto MaterialEditor::draw(bool *p_open) -> void {
  if (!(ImGui::Begin(translate("Material Editor"), p_open)) ||
      graph == nullptr) {
    ImGui::End();
    return;
  }
  ImNodes::EditorContextSet(imnodes_context);
  ImNodes::BeginNodeEditor();
  /// Draws nodes
  for (auto &node : graph->nodes) {
    draw_node(node.second.get());
  }
  // Draws links
  for (const auto &[uuid, link] : graph->links) {
    ImNodes::Link(link_id_map.create_or_get_imnodes_id(uuid),
                  attr_id_map.create_or_get_imnodes_id(link.from_socket),
                  attr_id_map.create_or_get_imnodes_id(link.to_socket));
  }
  for (auto &[uuid, comment_node] : graph->comments) {
    draw_comment_node(*comment_node.get());
  }
  ImNodes::EndNodeEditor();
  // Draw main context menu
  main_context_menu();
  check_for_new_links();
  check_for_deleted_links();
  check_for_deleted_nodes();
  ImGui::End();
}

auto MaterialEditor::draw_node(core::MaterialNode *node) -> void {
  auto &style = ImNodes::GetStyle();
  ImNodes::BeginNode(node_id_map.create_or_get_imnodes_id(node->uuid));
  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted(node->definition->ui_name.data());
  ImNodes::EndNodeTitleBar();

  // Draw input sockets
  for (auto &in : node->inputs) {
    switch (in.definition->type) {
      case MaterialSocketType::color:
        style.Colors[ImNodesCol_::ImNodesCol_Pin] = IM_COL32(255, 255, 0, 255);
        break;
      case MaterialSocketType::grayscale:
        style.Colors[ImNodesCol_::ImNodesCol_Pin] =
            IM_COL32(128, 128, 128, 255);
        break;
      case MaterialSocketType::universal:
        style.Colors[ImNodesCol_::ImNodesCol_Pin] =
            IM_COL32(255, 255, 255, 255);

        break;
    }
    ImNodes::BeginInputAttribute(attr_id_map.create_or_get_imnodes_id(in.uid));
    ImGui::TextUnformatted(in.definition->ui_name.data());
    ImNodes::EndInputAttribute();
  }

  // Draw output sockets
  for (auto &out : node->outputs) {
    switch (out.definition->type) {
      case MaterialSocketType::color:
        style.Colors[ImNodesCol_::ImNodesCol_Pin] = IM_COL32(255, 255, 0, 255);
        break;
      case MaterialSocketType::grayscale:
        style.Colors[ImNodesCol_::ImNodesCol_Pin] =
            IM_COL32(128, 128, 128, 255);
        break;
    }
    ImNodes::BeginOutputAttribute(
        attr_id_map.create_or_get_imnodes_id(out.uid));
    ImGui::TextUnformatted(out.definition->ui_name.data());
    ImNodes::EndOutputAttribute();
  }

  // Draw preview
  gl::GLuint tex = 0;
  if (!node->outputs.empty()) {
    const auto output_socket_buffer =
        materialEngineContext->get_buffer(node->uuid, node->outputs[0].uid);
    tex = output_socket_buffer.texture_id;
  }

  if (tex != 0) {
    // TODO Make the size of the texture configurable
    uintptr_t ptr = tex;
    ImGui::Image((ImTextureID)ptr,
                 {ImGui::GetFontSize() * 7, ImGui::GetFontSize() * 7});
  }
  ImNodes::EndNode();
  if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
    af_context->ui_context.main_window.parameter_editor.open(node);
  }
}

auto MaterialEditor::draw_comment_node(core::CommentNode &node) -> void {
  ImNodes::BeginNode(node_id_map.create_or_get_imnodes_id(node.uuid));
  ImGui::InputText("###comment", &node.comment);
  ImNodes::EndNode();
}

auto MaterialEditor::open_graph(core::MaterialGraph *new_graph) -> void {
  if (graph != nullptr) {
    close_graph();
  }

  if (materialEngineContext == nullptr) {
    materialEngineContext =
        std::make_unique<MaterialEngineContext>(&af_context->material_engine);
  }

  imnodes_context = ImNodes::EditorContextCreate();
  ImNodes::EditorContextSet(imnodes_context);
  graph = new_graph;
  materialEngineContext->load_graph(graph);

  for (const auto &[uuid, node_ptr] : graph->nodes) {
    ImNodes::SetNodeGridSpacePos(
        node_id_map.create_or_get_imnodes_id(uuid),
        {node_ptr->ui_info.x_pos, node_ptr->ui_info.y_pos});
  }
}

auto MaterialEditor::close_graph() -> void {
  ImNodes::EditorContextSet(imnodes_context);
  for (const auto &[uuid, node_ptr] : graph->nodes) {
    const auto node_pos =
        ImNodes::GetNodeGridSpacePos(node_id_map.get_imnodes_id(uuid));
    node_ptr->ui_info.x_pos = node_pos.x;
    node_ptr->ui_info.y_pos = node_pos.y;
  }
  materialEngineContext->unload_graph();

  graph = nullptr;
  ImNodes::EditorContextFree(imnodes_context);
  node_id_map.reset();
  attr_id_map.reset();
  link_id_map.reset();
}

auto MaterialEditor::main_context_menu() -> void {
  // const auto mouse_pos = ImGui::GetMousePos();
  int hovered_node = 0;
  if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) &&
      ImGui::IsMouseDown(ImGuiMouseButton_Right) &&
      !ImNodes::IsNodeHovered(&hovered_node)) {
    ImGui::OpenPopup("main_context_menu");
  }
  if (ImGui::BeginPopup("main_context_menu")) {
    if (ImGui::BeginMenu(translate("Add node"))) {
      if (material_nodes::node_definitions.empty())
        log::core_warn("Empty Node Definitions");

      for (const auto &material_node_definition :
           material_nodes::node_definitions) {
        prop_add_node_op(material_node_definition);
      }
      ImGui::EndMenu();
    }
    ImGui::EndPopup();
  }
}

auto MaterialEditor::check_for_new_links() -> void {
  int from_node = 0;
  int from_socket = 0;
  int to_node = 0;
  int to_socket = 0;
  if (ImNodes::IsLinkCreated(&from_node, &from_socket, &to_node, &to_socket)) {
    af_context->queue_operation<CreateMaterialGraphLink>(
        graph, node_id_map.get_uuid(from_node),
        attr_id_map.get_uuid(from_socket), node_id_map.get_uuid(to_node),
        attr_id_map.get_uuid(to_socket));
  }
}

auto MaterialEditor::check_for_deleted_links() -> void {
  // TODO delete the selected links too
  auto selected_links = std::vector<int>(ImNodes::NumSelectedLinks());
  if (!selected_links.empty() && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    for (const auto link_id : selected_links) {
      af_context->queue_operation<DeleteMaterialGraphLink>(
          graph, link_id_map.get_uuid(link_id));
    }
    ImNodes::ClearLinkSelection();
  }
  int link_id = 0;
  if (ImNodes::IsLinkDestroyed(&link_id)) {
    af_context->queue_operation<DeleteMaterialGraphLink>(
        graph, link_id_map.get_uuid(link_id));
  }
}

auto MaterialEditor::check_for_deleted_nodes() -> void {
  auto sel_nodes = std::vector<int>(ImNodes::NumSelectedNodes(), 0);
  if (sel_nodes.empty() || !ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    return;
  }
  ImNodes::GetSelectedNodes(sel_nodes.data());
  for (const auto node_id : sel_nodes) {
    af_context->queue_operation<DeleteNode>(graph,
                                            node_id_map.get_uuid(node_id));
  }
}

auto MaterialEditor::prop_add_node_op(
    const MaterialNodeDefinition &material_node_definition) const -> void {
  if (draw_operator<AddNode>(material_node_definition.ui_name, af_context,
                             material_node_definition.icon, graph,
                             &material_node_definition)) {
    ImGui::CloseCurrentPopup();
  }
}
}  // namespace afro::ui