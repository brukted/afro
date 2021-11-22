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
#include <imnodes.h>

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
MaterialEditor::MaterialEditor(core::Context *af_context) : af_context(af_context) {}

auto MaterialEditor::draw(bool *p_open) -> void {
  if (!(ImGui::Begin(translate("Material Editor"), p_open)) || graph == nullptr) {
    ImGui::End();
    return;
  }
  imnodes::EditorContextSet(imnodes_context);
  imnodes::BeginNodeEditor();
  /// Draws nodes
  for (auto &node : graph->nodes) {
    draw_node(node.second.get());
  }
  // Draws links
  for (const auto &[uuid, link] : graph->links) {
    imnodes::Link(link_id_map.create_or_get_int(uuid), attr_id_map.create_or_get_int(link.from_socket),
                  attr_id_map.create_or_get_int(link.to_socket));
  }
  for (auto &[uuid, comment_node] : graph->comments) {
    draw_comment_node(*comment_node.get());
  }
  imnodes::EndNodeEditor();
  // Draw main context menu
  main_context_menu();
  check_for_new_links();
  check_for_deleted_links();
  check_for_deleted_nodes();
  ImGui::End();
}

auto MaterialEditor::draw_node(core::MaterialNode *node) -> void {
  auto &style = imnodes::GetStyle();
  imnodes::BeginNode(node_id_map.create_or_get_int(node->uuid));
  imnodes::BeginNodeTitleBar();
  ImGui::TextUnformatted(node->ui_name.data());
  imnodes::EndNodeTitleBar();
  for (auto &in : node->inputs) {
    switch (in.type) {
      case MaterialSocketType::color:
        style.colors[imnodes::ColorStyle_Pin] = IM_COL32(255, 255, 0, 255);
        break;
      case MaterialSocketType::grayscale:
        style.colors[imnodes::ColorStyle_Pin] = IM_COL32(128, 128, 128, 255);
        break;
      case MaterialSocketType::universal:
        style.colors[imnodes::ColorStyle_Pin] = IM_COL32(255, 255, 255, 255);

        break;
    }
    imnodes::BeginInputAttribute(attr_id_map.create_or_get_int(in.uid));
    ImGui::TextUnformatted(in.ui_name.data());
    imnodes::EndInputAttribute();
  }
  for (auto &out : node->outputs) {
    switch (out.type) {
      case MaterialSocketType::color:
        style.colors[imnodes::ColorStyle_Pin] = IM_COL32(255, 255, 0, 255);
        break;
      case MaterialSocketType::grayscale:
        style.colors[imnodes::ColorStyle_Pin] = IM_COL32(128, 128, 128, 255);
        break;
    }
    imnodes::BeginOutputAttribute(attr_id_map.create_or_get_int(out.uid));
    ImGui::TextUnformatted(out.ui_name.data());
    imnodes::EndOutputAttribute();
  }
  gl::GLuint tex = 0;
  if (!node->outputs.empty()) {
    if (node->outputs[0].cache_tex.has_value()) {
      tex = node->outputs[0].cache_tex.value();
    }
  }
  if (tex != 0) {
    // TODO Make the size of the texture configurable
    uintptr_t ptr = tex;
    ImGui::Image((ImTextureID)ptr, {ImGui::GetFontSize() * 7, ImGui::GetFontSize() * 7});
  }
  imnodes::EndNode();
  if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
    af_context->ui_context.main_window.parameter_editor.open(node);
  }
}

auto MaterialEditor::draw_comment_node(core::CommentNode &node) -> void {
  imnodes::BeginNode(node_id_map.create_or_get_int(node.uuid));
  ImGui::InputText("###comment", &node.comment);
  imnodes::EndNode();
}

auto MaterialEditor::open_graph(core::MaterialGraph *new_graph) -> void {
  if (graph != nullptr) {
    close_graph();
  }
  if (!exe_context.has_value()) {
    exe_context = MaterialExecutionContext();
    exe_context.value().setup_proccesors();
  }
  imnodes_context = imnodes::EditorContextCreate();
  imnodes::EditorContextSet(imnodes_context);
  graph = new_graph;
  graph->init(&exe_context.value());
  for (const auto &[uuid, node_ptr] : graph->nodes) {
    imnodes::SetNodeGridSpacePos(node_id_map.create_or_get_int(uuid),
                                 {node_ptr->ui_info.x_pos, node_ptr->ui_info.y_pos});
  }
}

auto MaterialEditor::close_graph() -> void {
  imnodes::EditorContextSet(imnodes_context);
  for (const auto &[uuid, node_ptr] : graph->nodes) {
    const auto node_pos = imnodes::GetNodeGridSpacePos(node_id_map.get_int(uuid));
    node_ptr->ui_info.x_pos = node_pos.x;
    node_ptr->ui_info.y_pos = node_pos.y;
  }
  graph->deinit();
  graph = nullptr;
  imnodes::EditorContextFree(imnodes_context);
  node_id_map.reset();
  attr_id_map.reset();
  link_id_map.reset();
}

auto MaterialEditor::main_context_menu() -> void {
  // const auto mouse_pos = ImGui::GetMousePos();
  int hovered_node = 0;
  if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseDown(ImGuiMouseButton_Right) &&
      !imnodes::IsNodeHovered(&hovered_node)) {
    ImGui::OpenPopup("main_context_menu");
  }
  if (ImGui::BeginPopup("main_context_menu")) {
    if (ImGui::BeginMenu(translate("Add node"))) {
      prop_add_node_op<material_nodes::BlendNode>(translate("Blend"), Icon::BLEND_NODE);
      prop_add_node_op<material_nodes::BlurNode>(translate("Blur"), Icon::BLUR_NODE);
      prop_add_node_op<material_nodes::ChannelSelectNode>(translate("Channel Select"), Icon::CHANNELS_SHUFFLE_NODE);
      prop_add_node_op<material_nodes::TextNode>(translate("Text"), Icon::TEXT_NODE);
      prop_add_node_op<material_nodes::UniformColorNode>(translate("Uniform Color"), Icon::UNIFORM_COLOR_NODE);
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
  if (imnodes::IsLinkCreated(&from_node, &from_socket, &to_node, &to_socket)) {
    af_context->queue_operation<CreateMaterialGraphLink>(
        graph, node_id_map.get_uuid(from_node), attr_id_map.get_uuid(from_socket), node_id_map.get_uuid(to_node),
        attr_id_map.get_uuid(to_socket));
  }
}

auto MaterialEditor::check_for_deleted_links() -> void {
  // TODO delete the selected links too
  auto selected_links = std::vector<int>(imnodes::NumSelectedLinks());
  if (!selected_links.empty() && ImGui::IsKeyPressed(GLFW_KEY_DELETE)) {
    for (const auto link_id : selected_links) {
      af_context->queue_operation<DeleteMaterialGraphLink>(graph, link_id_map.get_uuid(link_id));
    }
    imnodes::ClearLinkSelection();
  }
  int link_id = 0;
  if (imnodes::IsLinkDestroyed(&link_id)) {
    af_context->queue_operation<DeleteMaterialGraphLink>(graph, link_id_map.get_uuid(link_id));
  }
}

auto MaterialEditor::check_for_deleted_nodes() -> void {
  auto sel_nodes = std::vector<int>(imnodes::NumSelectedNodes(), 0);
  if (sel_nodes.empty() || !ImGui::IsKeyPressed(GLFW_KEY_DELETE)) {
    return;
  }
  imnodes::GetSelectedNodes(sel_nodes.data());
  for (const auto node_id : sel_nodes) {
    af_context->queue_operation<DeleteNode>(graph, node_id_map.get_uuid(node_id));
  }
}

template <typename NodeType>
auto MaterialEditor::prop_add_node_op(const std::string_view name, const Icon icon) -> void {
  if (draw_operator<AddNode<NodeType>>(name, af_context, icon, graph, &exe_context.value())) {
    ImGui::CloseCurrentPopup();
  }
}
}  // namespace afro::ui