/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_graph_ops.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

#include "core/context.h"
#include "core/folder.h"
#include "core/material_graph.h"
#include "undo/operator.h"
#include "utils/asset.h"
#include "utils/log.h"
#include "utils/translation.h"

// TODO I am not sure if implementations this much functionality inside
// operators try refactoring into material_graph

namespace afro::core {
AddNewMaterialGraph::AddNewMaterialGraph(Context *context, Folder *folder)
    : Operator("MATERIAL_GRAPH_OP_CREATE"), context(context), folder(folder) {}

auto AddNewMaterialGraph::execute() -> void {
  auto ptr = std::make_unique<MaterialGraph>(generate_uuid());
  ptr->identifier = graph_name;
  graph_uid = ptr->uuid;
  folder->add_item(ptr.get());
  context->data.material_graphs.emplace_back(std::move(ptr));
}

auto AddNewMaterialGraph::undo() -> void {
  auto pos = std::remove_if(std::begin(context->data.material_graphs),
                            std::end(context->data.material_graphs),
                            [&](auto &val) { return val->uuid == graph_uid; });
  if (pos != std::end(context->data.material_graphs)) {
    graph_ptr = std::move(*pos);
    context->data.material_graphs.erase(pos);
    folder->remove_item(graph_ptr.get());
  } else {
    throw OperatorError("Graph doesn't exist");
  }
}

auto AddNewMaterialGraph::redo() -> void {
  folder->add_item(graph_ptr.get());
  context->data.material_graphs.emplace_back(std::move(graph_ptr));
}

auto AddNewMaterialGraph::draw() -> OperatorResult {
  OperatorResult result = OperatorResult::DRAWING_UI;
  ImGui::OpenPopup(translate("Material Graph"));
  if (ImGui::BeginPopupModal(translate("Material Graph"))) {
    ImGui::InputText(translate("Name"), &graph_name);
    if (ImGui::Button(translate("Create"))) {
      ImGui::CloseCurrentPopup();
      result = OperatorResult::FINISHED_UI;
    }
    ImGui::EndPopup();
  }
  return result;
}

DeleteMaterialGraph::DeleteMaterialGraph(Context *context, Folder *folder,
                                         UUID graph_uid)
    : Operator("MATERIAL_GRAPH_OP_DELETE"),
      context(context),
      folder(folder),
      graph_uid(graph_uid) {}

auto DeleteMaterialGraph::execute() -> void {
  auto &mat_graphs = context->data.material_graphs;
  auto pos =
      std::remove_if(std::begin(mat_graphs), std::end(mat_graphs),
                     [this](auto &val) { return val->uuid == graph_uid; });
  if (pos != std::end(mat_graphs)) {
    graph = std::move(*pos);
    try {
      folder->remove_item(graph.get());
    } catch (const std::runtime_error &e) {
      throw OperatorError(e.what());
    }
    mat_graphs.erase(pos);
  } else {
    throw OperatorError("Graph doesn't exist in material graphs");
  }
}

auto DeleteMaterialGraph::undo() -> void {
  folder->add_item(graph.get());
  context->data.material_graphs.push_back(std::move(graph));
}

auto DeleteMaterialGraph::redo() -> void { execute(); }

CreateMaterialGraphLink::CreateMaterialGraphLink(MaterialGraph *graph,
                                                 UUID from_node,
                                                 UUID from_socket, UUID to_node,
                                                 UUID to_socket)
    : Operator("MATERIAL_GRAPH_OP_CREATE_LK"),
      graph(graph),
      from_node(from_node),
      from_socket(from_socket),
      to_node(to_node),
      to_socket(to_socket) {}

auto CreateMaterialGraphLink::execute() -> void {
  const auto &node = graph->get_node(to_node);
  const auto dest_socket =
      std::find_if(node.inputs.begin(), node.inputs.end(),
                   [&](auto &val) { return val.uid == to_socket; });
  if (dest_socket->link_uuid.has_value()) {
    preexisting_link = graph->delete_link(dest_socket->link_uuid.value());
  }
  link_uid = graph->create_link(from_node, from_socket, to_node, to_socket);
}

auto CreateMaterialGraphLink::undo() -> void {
  link = graph->delete_link(link_uid);
  if (preexisting_link.has_value()) {
    graph->add_link(preexisting_link.value());
  }
}

auto CreateMaterialGraphLink::redo() -> void {
  if (preexisting_link.has_value()) {
    graph->delete_link(preexisting_link.value().uuid);
  }
  graph->add_link(link);
}

DeleteMaterialGraphLink::DeleteMaterialGraphLink(MaterialGraph *graph,
                                                 UUID link_uid)
    : Operator("MATERIAL_GRAPH_OP_DELETE_LK"),
      graph(graph),
      link_uid(link_uid) {}

auto DeleteMaterialGraphLink::execute() -> void {
  link = graph->delete_link(link_uid);
}

auto DeleteMaterialGraphLink::undo() -> void { graph->add_link(link); }

auto DeleteMaterialGraphLink::redo() -> void { execute(); }

DeleteNode::DeleteNode(MaterialGraph *graph, UUID node_uid)
    : Operator("MATERIAL_GRAPH_OP_DELETE_NODE"),
      graph(graph),
      node_uid(node_uid) {}

auto DeleteNode::execute() -> void {
  // Delete all links connected to this node
  auto &node_ref = graph->get_node(node_uid);
  for (const auto &socket : node_ref.inputs) {
    if (socket.link_uuid.has_value()) {
      auto link = graph->delete_link(socket.link_uuid.value());
      links.push_back(link);
    }
  }
  for (const auto &socket : node_ref.outputs) {
    for (const auto &link_uuid : socket.links) {
      auto link = graph->delete_link(link_uuid);
      links.push_back(link);
    }
  }

  // Delete the node
  node = graph->delete_node(node_uid);
}
auto DeleteNode::undo() -> void {
  graph->add_node(std::move(node));
  std::for_each(links.begin(), links.end(),
                [&](const auto &link) { graph->add_link(link); });
}
auto DeleteNode::redo() -> void { execute(); }

OpenMaterialGraph::OpenMaterialGraph(core::Context *const context,
                                     core::MaterialGraph *const graph)
    : Operator("MATERIAL_GRAPH_OP_OPEN"), context(context), graph(graph) {}

auto OpenMaterialGraph::execute() -> void {
  old_graph = context->ui_context.main_window.material_editor.graph;
  context->ui_context.main_window.material_editor.open_graph(graph);
}

auto OpenMaterialGraph::undo() -> void {
  if (old_graph != nullptr) {
    context->ui_context.main_window.material_editor.open_graph(graph);
  } else {
    context->ui_context.main_window.material_editor.close_graph();
  }
}

auto OpenMaterialGraph::redo() -> void { execute(); }

CloseMaterialGraph::CloseMaterialGraph(core::Context *context)
    : Operator("MATERIAL_GRAPH_OP_CLOSE"), context(context) {}

auto CloseMaterialGraph::undo() -> void {
  if (old_graph != nullptr) {
    context->ui_context.main_window.material_editor.open_graph(old_graph);
  } else {
    context->ui_context.main_window.material_editor.close_graph();
  }
}

auto CloseMaterialGraph::execute() -> void {
  old_graph = context->ui_context.main_window.material_editor.graph;
  context->ui_context.main_window.material_editor.close_graph();
}

auto CloseMaterialGraph::redo() -> void { execute(); }

}  // namespace afro::core