/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <optional>
#include <string_view>
#include <unordered_map>

#include "core/graph_common.h"
#include "core/material_graph.h"
#include "core/uuid.h"

namespace afro::core {
struct MaterialGraph;
struct Context;
struct MaterialNode;
class MaterialEngineContext;
}  // namespace afro::core

namespace afro::ui {
enum class Icon;
}

struct ImNodesEditorContext;

namespace afro::ui {
struct MaterialEditor {
  /**
   * @brief A bidirectional map between UUIDs and imnodes ids.
   *
   */
  struct IDMap {
   private:
    int last_i = 0;
    std::unordered_map<core::UUID, int> map1;
    std::unordered_map<int, core::UUID> map2;

   public:
    inline auto get_uuid(int imnodes_id) { return map2.at(imnodes_id); };
    inline auto get_imnodes_id(core::UUID uuid) { return map1.at(uuid); };
    inline auto create_or_get_imnodes_id(core::UUID uuid) {
      if (map1.contains(uuid)) {
        return map1.at(uuid);
      }
      ++last_i;
      map1[uuid] = last_i;
      map2[last_i] = uuid;
      return last_i;
    }
    inline auto reset() -> void {
      map1.clear();
      map2.clear();
    };
  };

  std::unique_ptr<core::MaterialEngineContext> materialEngineContext;

  core::MaterialGraph *graph = nullptr;
  ImNodesEditorContext *imnodes_context = nullptr;
  // Pointer to afro's context
  core::Context *af_context;
  // Bidirectional maps of UUID and imnodes' int ids
  IDMap node_id_map;
  IDMap link_id_map;
  IDMap attr_id_map;

  MaterialEditor(core::Context *af_context);
  void draw(bool *p_open);
  auto open_graph(core::MaterialGraph *graph) -> void;
  auto close_graph() -> void;
  // TODO [scripting] add state query methods for scripting

 private:
  auto draw_node(core::MaterialNode *node) -> void;
  auto draw_comment_node(core::CommentNode &node) -> void;
  auto main_context_menu() -> void;
  auto check_for_new_links() -> void;
  auto check_for_deleted_links() -> void;
  auto check_for_deleted_nodes() -> void;
  auto prop_add_node_op(
      const core::MaterialNodeDefinition &material_node_definition) const
      -> void;
};

}  // namespace afro::ui