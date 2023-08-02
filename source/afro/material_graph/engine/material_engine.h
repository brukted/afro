/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <fruit/fruit.h>

#include <memory>
#include <unordered_map>

#include "material_graph/data/material_graph.h"
#include "material_graph/data/material_node.h"
#include "material_processor.h"
#include "output_buffer.h"

namespace afro::graph::material {
class MaterialEngine {
 private:
  std::shared_ptr<MaterialGraph> graph_;
  std::unordered_map<std::string, std::shared_ptr<MaterialProcessor>>
      processors_;
  std::unordered_map<UUID, OutputBuffer> buffers_;
  std::unordered_map<UUID, bool> is_node_dirty;

  auto destroy_buffer(UUID prop_uuid) -> void;

  auto get_nodes_topologically_sorted()
      -> std::vector<std::shared_ptr<MaterialNode>>;

  auto mark_nodes_dirty(UUID start_node_uuid) -> void;

 public:
  INJECT(MaterialEngine()) = default;

  auto create_or_get_processor(MaterialNodeDefinition const& node_def)
      -> std::shared_ptr<MaterialProcessor>;

  auto create_or_get_buffer(UUID prop_uuid, int width, int height,
                            gl::GLenum format) -> OutputBuffer&;

  auto set_graph(std::shared_ptr<MaterialGraph> graph) -> void;
  auto clear_graph() -> void;
  auto update() -> void;
  auto on_node_created(std::shared_ptr<MaterialNode>) -> void;
  auto on_node_changed(UUID node_uuid) -> void;
  auto on_node_deleted(std::shared_ptr<MaterialNode>) -> void;
  auto on_link_created(Link link) -> void;
  auto on_link_deleted(Link link) -> void;

  auto get_preview_texture(MaterialNode& node) -> gl::GLuint;
};
}  // namespace afro::graph::material