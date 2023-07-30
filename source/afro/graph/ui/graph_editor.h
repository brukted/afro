/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>
#include <string>

#include "graph/data/graph.h"
#include "id_map.h"
#include "ui/interfaces/widget.h"
#include "undo/interfaces/undo_stack.h"

struct ImNodesEditorContext;

namespace afro::graph {
class GraphEditor : public ui::Widget {
 protected:
  std::shared_ptr<Graph> graph;
  virtual auto draw_node_body(Node& node) -> void = 0;
  virtual auto draw_main_context_menu() -> void = 0;

 private:
  std::shared_ptr<undo::UndoStack> undo_stack;
  std::string name = "Graph Editor";
  // Bidirectional maps of UUID and imnodes' integer ids
  IDMap node_id_map;
  IDMap link_id_map;
  IDMap attr_id_map;
  ImNodesEditorContext* imnodes_context = nullptr;
  bool show_debug_node_id = false;
  bool show_debug_property_id = false;
  auto check_for_new_links() -> void;
  auto check_for_deleted_links() -> void;
  auto check_for_deleted_nodes() -> void;
  auto draw_node(Node& node) -> void;
  auto draw_property(const property::Property& property) -> void;

 public:
  GraphEditor(std::string name, std::shared_ptr<undo::UndoStack> undo_stack)
      : undo_stack(std::move(undo_stack)), name(std::move(name)) {}

  auto draw() -> void override;
  auto set_graph(std::shared_ptr<Graph> graph) -> void;
  auto clear_graph() -> void;

  ~GraphEditor() override = default;
};

}  // namespace afro::graph