/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_engine.h"

#include <queue>

#include "utils/assert.h"

namespace afro::graph::material {
auto MaterialEngine::set_graph(std::shared_ptr<MaterialGraph> graph) -> void {
  AF_ASSERT_MSG(graph != nullptr, "graph is null");
  AF_ASSERT_MSG(graph_ == nullptr, "graph is already set")
  graph_ = graph;

  // Mark all nodes as dirty
  for (auto &node : graph->get_nodes()) {
    AF_ASSERT(node != nullptr)
    std::shared_ptr<MaterialNode> material_node =
        std::dynamic_pointer_cast<MaterialNode>(node);
    is_node_dirty[material_node->get_uuid()] = true;
  }
}

auto MaterialEngine::clear_graph() -> void {
  graph_ = nullptr;
  buffers_.clear();
  is_node_dirty.clear();
}

auto MaterialEngine::update() -> void {
  auto nodes = get_nodes_topologically_sorted();

  for (auto &node : nodes) {
    if (is_node_dirty[node->get_uuid()]) {
      log::core_trace("Executing node: {}", node->get_uuid());
      MaterialNodeExecFun &exec_fun = node->get_definition().get_on_execute();
      exec_fun(this, graph_.get(), node.get());
      is_node_dirty[node->get_uuid()] = false;
    }
  }
}

auto MaterialEngine::get_nodes_topologically_sorted()
    -> std::vector<std::shared_ptr<MaterialNode>> {
  std::unordered_map<UUID, unsigned int> dependencies;
  auto order = std::vector<std::shared_ptr<MaterialNode>>();
  auto queue = std::queue<std::shared_ptr<MaterialNode>>();

  for (auto &node_ptr : graph_->get_nodes()) {
    std::shared_ptr<MaterialNode> node =
        std::dynamic_pointer_cast<MaterialNode>(node_ptr);
    dependencies[node->get_uuid()] =
        graph_->get_links_to_node(node->get_uuid()).size();
    if (dependencies[node->get_uuid()] == 0) {
      queue.push(std::move(node));
    }
  }

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop();
    order.push_back(node);

    auto links = graph_->get_links_from_node(node->get_uuid());

    for (auto &link : links) {
      auto to_node = graph_->get_node_by_uuid(link.get_to_node());
      AF_ASSERT(to_node != nullptr)
      std::shared_ptr<MaterialNode> material_node =
          std::dynamic_pointer_cast<MaterialNode>(to_node);
      dependencies[material_node->get_uuid()]--;
      if (dependencies[material_node->get_uuid()] == 0) {
        queue.push(std::move(material_node));
      }
    }
  }

  return order;
}

auto MaterialEngine::create_or_get_processor(
    const MaterialNodeDefinition &node_def)
    -> std::shared_ptr<MaterialProcessor> {
  auto iter = processors_.find(node_def.get_id());
  if (iter != processors_.end()) {
    return iter->second;
  }

  auto processor = std::make_shared<MaterialProcessor>();
  processor->init(node_def.get_shader_code());
  processors_[node_def.get_id()] = processor;
  return processor;
}

auto MaterialEngine::create_or_get_buffer(UUID uuid, int width, int height,
                                          gl::GLenum format) -> OutputBuffer & {
  auto iter = buffers_.find(uuid);
  if (iter != buffers_.end()) {
    return iter->second;
  }
  gl::GLuint texture = 0;
  gl::GLuint frame_buffer = 0;

  gl43core::glGenTextures(1, &texture);
  glBindTexture(gl::GL_TEXTURE_2D, texture);
  glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_S,
                  gl::GL_CLAMP_TO_EDGE);
  glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_T,
                  gl::GL_CLAMP_TO_EDGE);
  glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
  glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);

  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGBA, width, height, 0, format,
                   gl::GL_UNSIGNED_BYTE, nullptr);
  // TODO Swizzle G & B to R for grayscale

  gl::glGenFramebuffers(1, &frame_buffer);
  glBindFramebuffer(gl::GL_FRAMEBUFFER, frame_buffer);
  glFramebufferTexture2D(gl::GL_FRAMEBUFFER, gl::GL_COLOR_ATTACHMENT0,
                         gl::GL_TEXTURE_2D, texture, 0);
  glBindTexture(gl::GL_TEXTURE_2D, texture);
  glGenerateMipmap(gl::GL_TEXTURE_2D);
  glBindTexture(gl::GL_TEXTURE_2D, 0);

  buffers_[uuid] = {texture, frame_buffer};
  return buffers_[uuid];
}

auto MaterialEngine::destroy_buffer(afro::UUID prop_id) -> void {
  auto iter = buffers_.find(prop_id);
  AF_ASSERT_MSG(iter != buffers_.end(), "Buffer does not exist")

  auto buffer = iter->second;
  gl::glDeleteTextures(1, &buffer.texture_id);
  gl::glDeleteFramebuffers(1, &buffer.frame_buffer_id);

  buffers_.erase(iter);
}

auto MaterialEngine::mark_nodes_dirty(afro::UUID start_node_uuid) -> void {
  is_node_dirty[start_node_uuid] = true;
  auto links = graph_->get_links_from_node(start_node_uuid);

  for (auto &link : links) {
    mark_nodes_dirty(link.get_to_node());
  }
}

auto MaterialEngine::on_node_created(std::shared_ptr<MaterialNode> node)
    -> void {
  mark_nodes_dirty(node->get_uuid());
}

auto MaterialEngine::on_node_changed(UUID node_uuid) -> void {
  mark_nodes_dirty(node_uuid);
}

auto MaterialEngine::on_node_deleted(std::shared_ptr<MaterialNode> node)
    -> void {
  for (auto &prop : node->get_properties()) {
    if (prop.get_property_definition().type == property::Type::OUTPUT) {
      destroy_buffer(prop.get_uuid());
    }
  }
}

auto MaterialEngine::on_link_created(Link link) -> void {
  mark_nodes_dirty(link.get_to_node());
}

auto MaterialEngine::on_link_deleted(Link link) -> void {
  mark_nodes_dirty(link.get_to_node());
}

auto MaterialEngine::get_preview_texture(MaterialNode &node) -> gl::GLuint {
  for (const auto &prop : node.get_properties()) {
    if (prop.get_property_definition().type == property::Type::OUTPUT) {
      auto buffer = create_or_get_buffer(
          prop.get_uuid(), node.get_buffer_size().x, node.get_buffer_size().y,
          node.get_buffer_format());
      return buffer.texture_id;
    }
  }

  return 0;
}
}  // namespace afro::graph::material