/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <unordered_map>

#include "engine_utils.h"
#include "material_graph.h"
#include "material_node_definition.h"
#include "material_processor.h"

namespace afro::core {

struct MaterialGraph;
struct MaterialNode;

/**
 * @brief Finds the appropriate OpenGL texture format for the given socket type
 * and pixel_format
 *
 */
auto get_gl_format(MaterialSocketType socket_type, PixelFormat pixel_format)
    -> gl::GLenum;

class MaterialEngine {
 private:
  MaterialProcessor uniform_color_processor;
  MaterialProcessor blend_processor;
  MaterialProcessor blur_processor;
  MaterialProcessor channel_select_processor;

  auto setup_processors() -> void;
  auto destroy_processors() -> void;

 public:
  void init();
  void deinit();
};

class MaterialEngineContext {
 private:
  MaterialEngine* material_engine;
  std::map<UUID, std::map<UUID, OutputSocketBuffer>> node_buffers;
  std::optional<MaterialGraph*> loaded_graph;

  auto generate_output_buffers(MaterialNode* node) -> void;
  auto delete_output_buffers(MaterialNode* node) -> void;

  auto execute_forward(UUID start_node_id) -> void;
  auto execute_graph() -> void;

  auto generate_graph_output_buffers() -> void;
  auto delete_graph_output_buffers() -> void;

  auto node_added(UUID uuid) -> void;
  void link_added(UUID uuid);
  void link_deleted(UUID uuid);
  void node_deleted(std::vector<UUID> dependent_nodes);

 public:
  MaterialEngineContext(MaterialEngine* materialEngine);

  [[nodiscard]] auto get_material_engine() const -> MaterialEngine* {
    return material_engine;
  }

  auto load_graph(MaterialGraph* graph) -> void;
  auto unload_graph() -> void;

  auto get_buffer(UUID node_id, UUID output_socket_id) -> OutputSocketBuffer;
  virtual ~MaterialEngineContext();
};
}  // namespace afro::core