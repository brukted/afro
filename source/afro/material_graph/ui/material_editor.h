/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <fruit/fruit.h>

#include <boost/signals2/signal.hpp>
#include <utility>

#include "graph/ui/graph_editor.h"
#include "material_graph/data/material_graph.h"
#include "material_graph/definitions/definitions.h"
#include "material_graph/engine/material_engine.h"
#include "ui/interfaces/widget.h"
#include "undo/interfaces/undo_stack.h"

namespace afro::graph::material {
class MaterialEditor : public GraphEditor {
 private:
  std::shared_ptr<undo::UndoStack> undo_stack;
  std::shared_ptr<MaterialEngine> engine;
  std::vector<boost::signals2::connection> connections;
  std::shared_ptr<NodeDefinitions> node_definitions;

 protected:
  auto draw_node_body(Node& node) -> void override;
  auto draw_main_context_menu() -> void override;

 public:
  INJECT(MaterialEditor(std::shared_ptr<undo::UndoStack> undo_stack_,
                        std::shared_ptr<property::PropertyEditor> props_editor,
                        std::shared_ptr<MaterialEngine> engine_,
                        std::shared_ptr<NodeDefinitions> node_definitions_))
      : GraphEditor("Material Editor", undo_stack_, std::move(props_editor)),
        undo_stack(std::move(undo_stack_)),
        engine(std::move(engine_)),
        node_definitions(std::move(node_definitions_)) {}

  auto set_graph(std::shared_ptr<MaterialGraph> graph) -> void;
  auto clear_graph() -> void override;
  auto draw() -> void override;
  auto shutdown() -> void;
  
  ~MaterialEditor() override = default;
};
}  // namespace afro::graph::material