/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "graph_editor.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "graph/commands/add_link_command.h"
#include "graph/commands/delete_links_command.h"
#include "imnodes/imnodes.h"
#include "utils/assert.h"
#include "utils/translation.h"

namespace afro::graph {
auto GraphEditor::draw() -> void {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(translate("Editors"))) {
      if (ImGui::BeginMenu(translate("Graph Editor"))) {
        ImGui::Selectable(translate("Show Node Ids"), &show_debug_node_id);
        ImGui::Selectable(translate("Show Socket Ids"),
                          &show_debug_property_id);
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // FIXME: this is not working
  if (!ImGui::Begin(name.c_str(), &show) || !graph) {
    ImGui::End();
    return;
  }

  ImNodes::EditorContextSet(imnodes_context);
  ImNodes::BeginNodeEditor();
  // Draw nodes
  for (auto node : graph->get_nodes()) {
    draw_node(node);
  }

  // Draw links
  for (const auto &link : graph->get_links()) {
    ImNodes::Link(
        link_id_map.create_or_get_imnodes_id(link.get_uuid()),
        attr_id_map.create_or_get_imnodes_id(link.get_from_property()),
        attr_id_map.create_or_get_imnodes_id(link.get_to_property()));
  }

  // TODO: Draw graph items
  ImNodes::MiniMap(
      0.2F,  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      ImNodesMiniMapLocation_BottomRight);

  ImNodes::EndNodeEditor();

  draw_main_context_menu();
  check_for_new_links();
  check_for_deleted_links();
  check_for_deleted_nodes();

  ImGui::End();
}

auto GraphEditor::draw_node(std::shared_ptr<Node> node) -> void {
  ImNodes::BeginNode(node_id_map.create_or_get_imnodes_id(node->get_uuid()));

  ImNodes::BeginNodeTitleBar();
  if (show_debug_node_id) {
    ImGui::Text("%s\n (%llu)", node->get_name().data(), node->get_uuid());
  } else {
    ImGui::TextUnformatted(node->get_name().data());
  }
  ImNodes::EndNodeTitleBar();

  for (const auto &property : node->get_properties()) {
    draw_property(property);
  }
  draw_node_body(*node);

  ImNodes::EndNode();

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
    props_editor->set_object(node);
  }
}

auto GraphEditor::check_for_new_links() -> void {
  int from_node = 0;
  int from_socket = 0;
  int to_node = 0;
  int to_socket = 0;
  if (ImNodes::IsLinkCreated(&from_node, &from_socket, &to_node, &to_socket)) {
    auto from_node_uuid = node_id_map.get_uuid(from_node);
    auto to_node_uuid = node_id_map.get_uuid(to_node);
    auto from_socket_uuid = attr_id_map.get_uuid(from_socket);
    auto to_socket_uuid = attr_id_map.get_uuid(to_socket);

    undo_stack->enqueue(std::make_unique<AddLinkCommand>(
        graph, Link({from_node_uuid, from_socket_uuid},
                    {to_node_uuid, to_socket_uuid})));

    log::core_debug("Created link from {}:{} to {}:{}", from_node_uuid,
                    from_socket_uuid, to_node_uuid, to_socket_uuid);
  }
}

auto GraphEditor::check_for_deleted_links() -> void {
  auto selected_links = ImNodes::NumSelectedLinks();

  if (selected_links != 0 && (ImGui::IsKeyPressed(ImGuiKey_Delete) ||
                              ImGui::IsKeyPressed(ImGuiKey_Backspace))) {
    auto selected_links_id = std::vector<int>(selected_links);
    ImNodes::GetSelectedLinks(selected_links_id.data());

    auto link_uuids = std::vector<UUID>();
    for (const auto link_id : selected_links_id) {
      link_uuids.push_back(link_id_map.get_uuid(link_id));
    }

    undo_stack->enqueue(std::make_unique<DeleteLinks>(
        graph, graph->get_links_by_uuids(link_uuids)));
    std::string link_uuids_str;
    for (const auto &link_uuid : link_uuids) {
      link_uuids_str += fmt::format("{} ", link_uuid);
    }
    log::core_debug("Deleted links : [{}] ", link_uuids_str);
    ImNodes::ClearLinkSelection();
  }

  int link_id = 0;
  if (ImNodes::IsLinkDestroyed(&link_id)) {
    auto link_uuid = link_id_map.get_uuid(link_id);
    undo_stack->enqueue(std::make_unique<DeleteLinks>(
        graph, graph->get_links_by_uuids({link_uuid})));
    log::core_debug("Deleted link {}", link_uuid);
  }
}

auto GraphEditor::check_for_deleted_nodes() -> void {
  auto sel_nodes = std::vector<int>(ImNodes::NumSelectedNodes(), 0);
  if (sel_nodes.empty() || !ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    return;
  }

  ImNodes::GetSelectedNodes(sel_nodes.data());
  for (const auto node_id : sel_nodes) {
    // TODO: Delete node
    auto node_uuid = node_id_map.get_uuid(node_id);
    log::core_debug("Deleting node {}", node_uuid);
  }
}

auto GraphEditor::set_graph(std::shared_ptr<Graph> new_graph) -> void {
  AF_ASSERT(new_graph != nullptr)

  if (graph != nullptr) {
    clear_graph();
  }

  graph = new_graph;

  AF_ASSERT(imnodes_context == nullptr)
  imnodes_context = ImNodes::EditorContextCreate();
  ImNodes::EditorContextSet(imnodes_context);

  for (auto &node : graph->get_nodes()) {
    const auto node_id = node_id_map.create_or_get_imnodes_id(node->get_uuid());
    ImNodes::SetNodeGridSpacePos(node_id,
                                 ImVec2(node->position.x, node->position.y));
  }
}

auto GraphEditor::clear_graph() -> void {
  ImNodes::EditorContextSet(imnodes_context);

  for (const auto &node_ptr : graph->get_nodes()) {
    const auto node_pos = ImNodes::GetNodeGridSpacePos(
        node_id_map.create_or_get_imnodes_id(node_ptr->get_uuid()));
    node_ptr->position.x = node_pos.x;
    node_ptr->position.y = node_pos.y;
  }

  graph = nullptr;
  ImNodes::EditorContextFree(imnodes_context);
  imnodes_context = nullptr;

  // Clear maps
  node_id_map.clear();
  link_id_map.clear();
  attr_id_map.clear();
}

auto GraphEditor::draw_property(const property::Property &property) -> void {
  auto &style = ImNodes::GetStyle();
  constexpr auto is_input_socket = [](const property::Property &prop) {
    return prop.get_property_definition().type == property::Type::INPUT &&
           prop.get_property_definition().is_socket;
  };

  constexpr auto is_output_socket = [](const property::Property &prop) {
    return prop.get_property_definition().type == property::Type::OUTPUT &&
           prop.get_property_definition().is_socket;
  };

  switch (property.get_property_definition().value_type) {
    case property::ValueType::FLOAT:
      style.Colors[ImNodesCol_::ImNodesCol_Pin] = IM_COL32(128, 128, 128, 255);
      break;
    case property::ValueType::FLOAT_4:
    case property::ValueType::FLOAT_3:
      style.Colors[ImNodesCol_::ImNodesCol_Pin] = IM_COL32(255, 255, 0, 255);
      break;
    default:
      AF_ASSERT_MSG(false, "Unknown property value type")
  }

  is_input_socket(property)
      ? ImNodes::BeginInputAttribute(
            attr_id_map.create_or_get_imnodes_id(property.get_uuid()))
      : ImNodes::BeginOutputAttribute(
            attr_id_map.create_or_get_imnodes_id(property.get_uuid()));

  if (show_debug_property_id) {
    ImGui::Text("%s\n (%llu)", property.get_property_definition().name.data(),
                property.get_uuid());
  } else {
    ImGui::TextUnformatted(property.get_property_definition().name.data());
  }

  is_input_socket(property) ? ImNodes::EndInputAttribute()
                            : ImNodes::EndOutputAttribute();
}
}  // namespace afro::graph
