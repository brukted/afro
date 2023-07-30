/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "common/interfaces/command.h"
#include "graph/data/graph.h"
#include "graph/data/node.h"
#include "material_graph/data/material_node.h"
#include "material_graph/data/material_node_definition.h"

namespace afro::graph {

class AddNode : public Command {
 private:
  material::MaterialNodeDefinition node_definition;
  std::shared_ptr<Node> node;
  std::shared_ptr<Graph> graph;

 public:
  AddNode(material::MaterialNodeDefinition node_definition,
          std::shared_ptr<Graph> graph)
      : Command("ADD_NODE"),
        node_definition(std::move(node_definition)),
        node(material::MaterialNode::create(node_definition)),
        graph(std::move(graph)) {}

  auto execute() -> void override { graph->add_node(node); }
  auto undo() -> void override { graph->remove_node_by_uuid(node->get_uuid()); }
  ~AddNode() override = default;
};

}  // namespace afro::graph