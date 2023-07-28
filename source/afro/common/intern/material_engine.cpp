/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/material_engine.h"

#include <glbinding/gl41core/gl.h>

#include <utility>

#include "core/engine_utils.h"
#include "core/material_nodes.h"
#include "utils/embed_data.h"
#include "utils/log.h"

EMBEDDED_DATA(mat_vertex_vert)
EMBEDDED_DATA(uniform_color_frag)
EMBEDDED_DATA(blend_frag)
EMBEDDED_DATA(blur_frag)
EMBEDDED_DATA(channel_select_frag)

using namespace gl;

namespace afro::core {
void MaterialEngine::init() {
  log::core_trace("Initializing material engine");
  setup_processors();
  material_nodes::register_node_definitions();
  log::core_trace("Material engine initialized");
}
void MaterialEngine::deinit() {
  log::core_trace("Deinitilizing material engine");
  destroy_processors();
  log::core_trace("Material engine deinitialized");
}
auto MaterialEngine::setup_processors() -> void {
  uniform_color_processor.init(embed_data_mat_vertex_vert,
                               embed_data_uniform_color_frag);
  blend_processor.init(embed_data_mat_vertex_vert, embed_data_blend_frag);
  blur_processor.init(embed_data_mat_vertex_vert, embed_data_blur_frag);
  channel_select_processor.init(embed_data_mat_vertex_vert,
                                embed_data_channel_select_frag);
}
auto MaterialEngine::destroy_processors() -> void {
  uniform_color_processor.deinit();
  blend_processor.deinit();
  blur_processor.deinit();
  channel_select_processor.deinit();
}

auto MaterialEngineContext::load_graph(MaterialGraph *graph) -> void {
  AF_ASSERT(!loaded_graph.has_value())
  loaded_graph = graph;
  graph->link_added_callback = [&](UUID uuid) { link_added(uuid); };

  graph->link_deleted_callback = [&](UUID uuid) { link_deleted(uuid); };

  graph->node_added_callback = [&](UUID uuid) { node_added(uuid); };

  graph->node_deleted_callback = [&](std::vector<UUID> dependent_nodes) {
    node_deleted(std::move(dependent_nodes));
  };

  generate_graph_output_buffers();
  execute_graph();
}

auto MaterialEngineContext::unload_graph() -> void {
  AF_ASSERT(loaded_graph.has_value())
  auto *graph = loaded_graph.value();
  graph->link_added_callback.reset();
  graph->link_deleted_callback.reset();
  graph->node_added_callback.reset();
  graph->node_deleted_callback.reset();
  delete_graph_output_buffers();
  loaded_graph.reset();
}

MaterialEngineContext::~MaterialEngineContext(){
    AF_ASSERT(!loaded_graph.has_value()) AF_ASSERT(!node_buffers.empty())}

MaterialEngineContext::MaterialEngineContext(MaterialEngine *materialEngine)
    : material_engine(materialEngine) {}

auto MaterialEngineContext::delete_graph_output_buffers() -> void {
  for (const auto &node : loaded_graph.value()->nodes) {
    delete_output_buffers(node.second.get());
  }
}

auto MaterialEngineContext::delete_output_buffers(MaterialNode *node) -> void {
  AF_ASSERT(node_buffers.contains(node->uuid))

  const auto &node_buffer = node_buffers[node->uuid];

  for (const auto &[output_socket_id, buffer] : node_buffer) {
    gl::glDeleteTextures(1, &buffer.texture_id);
    gl::glDeleteFramebuffers(1, &buffer.frame_buffer_id);
  }

  node_buffers.erase(node->uuid);
}

auto MaterialEngineContext::generate_output_buffers(MaterialNode *node)
    -> void {
  AF_ASSERT(!node_buffers.contains(node->uuid))

  std::map<UUID, OutputSocketBuffer> &node_buffer = node_buffers[node->uuid];

  for (auto &output : node->outputs) {
    gl::GLuint texture = 0;
    gl::GLuint framebuffer = 0;
    //    const auto pixel_format =
    //        common_props.get_prop<EnumProperty>("pixel_format").value();

    const gl::GLenum internalformat =
        get_gl_format(output.definition->type, PixelFormat::eight_bit);

    glGenTextures(1, &texture);
    glBindTexture(gl::GL_TEXTURE_2D, texture);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_S,
                    gl::GL_CLAMP_TO_EDGE);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_T,
                    gl::GL_CLAMP_TO_EDGE);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER,
                    gl::GL_NEAREST);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER,
                    gl::GL_NEAREST);

    const int width = 1024, height = 1024;

    gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, internalformat, width, height, 0,
                     gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    // TODO Swizzle G & B to R for grayscale

    gl::glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(gl::GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(gl::GL_FRAMEBUFFER, gl::GL_COLOR_ATTACHMENT0,
                           gl::GL_TEXTURE_2D, texture, 0);
    glBindTexture(gl::GL_TEXTURE_2D, texture);
    glGenerateMipmap(gl::GL_TEXTURE_2D);
    glBindTexture(gl::GL_TEXTURE_2D, 0);

    node_buffer[output.uid] = OutputSocketBuffer(texture, framebuffer);
  }
}

auto MaterialEngineContext::generate_graph_output_buffers() -> void {
  for (const auto &node : loaded_graph.value()->nodes) {
    generate_output_buffers(node.second.get());
  }
}

auto MaterialEngineContext::execute_forward(UUID start_node_id) -> void {}

auto MaterialEngineContext::execute_graph() -> void {}

auto MaterialEngineContext::node_added(UUID uuid) -> void {
  generate_output_buffers(loaded_graph.value()->nodes[uuid].get());
  execute_forward(uuid);
}

void MaterialEngineContext::node_deleted(std::vector<UUID> dependent_nodes) {}

auto MaterialEngineContext::get_buffer(UUID node_id, UUID output_socket_id)
    -> OutputSocketBuffer {
  AF_ASSERT(node_buffers.contains(node_id))
  auto &node_buffer = node_buffers.at(node_id);
  return node_buffer[output_socket_id];
}

void MaterialEngineContext::link_added(UUID uuid) {}
void MaterialEngineContext::link_deleted(UUID uuid) {}

auto get_gl_format(MaterialSocketType socket_type, PixelFormat pixel_format)
    -> gl::GLenum {
  if (socket_type == MaterialSocketType::color) {
    switch (pixel_format) {
      case PixelFormat::eight_bit:
        return gl::GL_RGBA;
      case PixelFormat::sixteen_bit:
        return gl::GL_RGBA16;
      case PixelFormat::sixteen_bit_float:
        return gl::GL_RGBA16F;
      case PixelFormat::thirty_two_bit_float:
        return gl::GL_RGBA32F;
    }
  } else if (socket_type == MaterialSocketType::grayscale) {
    switch (pixel_format) {
      case PixelFormat::eight_bit:
        return gl::GL_R;
      case PixelFormat::sixteen_bit:
        return gl::GL_R16;
      case PixelFormat::sixteen_bit_float:
        return gl::GL_R16F;
      case PixelFormat::thirty_two_bit_float:
        return gl::GL_R32F;
    }
  }
  AF_ASSERT_MSG(false, "Unknown combination")
};
}  // namespace afro::core