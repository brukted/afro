/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <core/uuid.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/material_graph.h"
#include "operator.h"
#include "utils/translation.h"

namespace afro::core {

struct Context;
struct Folder;

struct AddNewMaterialGraph : Operator {
  Context *const context;
  Folder *const folder;
  UUID graph_uid = static_cast<UUID>(0);
  std::unique_ptr<MaterialGraph> graph_ptr = nullptr;
  std::string graph_name{translate("Untitled")};
  AddNewMaterialGraph(Context *context, Folder *folder);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
  auto draw() -> OperatorResult override;
};

struct DeleteMaterialGraph : Operator {
  Context *const context;
  Folder *const folder;
  const UUID graph_uid;
  std::unique_ptr<MaterialGraph> graph = nullptr;
  DeleteMaterialGraph(Context *context, Folder *folder, UUID graph_uid);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
};

struct CreateMaterialGraphLink : Operator {
  MaterialGraph *const graph;
  const UUID from_node;
  const UUID from_socket;
  const UUID to_node;
  const UUID to_socket;
  UUID link_uid = 0;
  MaterialGraphLink link{};  // to store the link after undoing
  std::optional<MaterialGraphLink> preexisting_link;
  CreateMaterialGraphLink(MaterialGraph *graph, UUID from_node, UUID from_socket, UUID to_node, UUID to_socket);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
};

struct DeleteMaterialGraphLink : Operator {
  MaterialGraph *const graph;
  const UUID link_uid;
  MaterialGraphLink link{};
  DeleteMaterialGraphLink(MaterialGraph *graph, UUID link_uid);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
};

template <typename NodeType>
struct AddNode : Operator {
  MaterialGraph *const graph;
  std::unique_ptr<MaterialNode> node = nullptr;
  MaterialExecutionContext *exe_context;
  UUID node_uuid = 0;
  AddNode(MaterialGraph *graph, MaterialExecutionContext *exe_context)
      : Operator("MATERIAL_GRAPH_OP_ADD_NODE"), graph(graph), exe_context(exe_context){};

  auto execute() -> void override {
    auto l_node = std::make_unique<NodeType>(generate_uuid(), graph);
    node_uuid = l_node->uuid;
    graph->add_node(std::move(l_node));
    graph->get_node(node_uuid).initialize(exe_context);
    graph->execute_forward(node_uuid);
  };

  auto undo() -> void override {
    auto flatten = graph->flatten(&graph->get_node(node_uuid));
    node = graph->delete_node(node_uuid);
    std::for_each(flatten.begin(), flatten.end(), [&](auto *node) {
      if (node->uuid != node_uuid) {
        node->execute();
      }
    });
  };

  auto redo() -> void override {
    graph->add_node(std::move(node));
    graph->execute_forward(node_uuid);
  };
};

struct DeleteNode : Operator {
  MaterialGraph *graph;
  UUID node_uid = 0;
  std::unique_ptr<MaterialNode> node = nullptr;
  std::vector<MaterialGraphLink> links{};
  DeleteNode(MaterialGraph *graph, UUID node_uid);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
};

struct OpenMaterialGraph : Operator {
  core::Context *const context;
  core::MaterialGraph *const graph;
  core::MaterialGraph *old_graph = nullptr;
  OpenMaterialGraph(core::Context *context, core::MaterialGraph *graph);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
};

struct CloseMaterialGraph : Operator {
  core::Context *const context;
  core::MaterialGraph *old_graph = nullptr;
  CloseMaterialGraph(core::Context *context);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
};

}  // namespace afro::core