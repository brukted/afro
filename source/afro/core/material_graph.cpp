/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_graph.h"

#include <glbinding/gl43core/gl.h>

#include <algorithm>
#include <cstdio>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <xutility>

#include "core/property.h"
#include "core/uuid.h"
#include "glbinding/gl/functions.h"
#include "glbinding/gl/types.h"
#include "utils/asset.h"
#include "utils/embed_data.h"

namespace ranges = std::ranges;
using namespace gl;

namespace afro::core {

auto MaterialProcessor::init(std::string_view vertex_shader, std::string_view fragment_shader) -> void {
  const auto *vertex_source = vertex_shader.data();
  const auto *fragment_source = fragment_shader.data();
  auto vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, nullptr);
  glCompileShader(vertex);
  auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_source, nullptr);
  glCompileShader(fragment);
  program_id = glCreateProgram();  // NOLINT(cppcoreguidelines-prefer-member-initializer)
  glAttachShader(program_id, vertex);
  glAttachShader(program_id, fragment);
  glLinkProgram(program_id);
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES),
               VERTICES,  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
               GL_STATIC_DRAW);

  const auto stride_size = 5;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(GL_FLOAT), (void *)nullptr);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(GL_FLOAT), reinterpret_cast<void *>(3 * 4));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
  is_initialized = true;
}

auto MaterialProcessor::execute(gl::GLuint output_frame_buf, GLuint tex_buf, int width, int height) const -> void {
  glUseProgram(program_id);
  glBindFramebuffer(gl::GLenum::GL_FRAMEBUFFER, output_frame_buf);
  glViewport(0, 0, width, height);
  glBindVertexArray(vao);
  constexpr GLsizei num_indices = 6;
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, tex_buf);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name, const Float4Property &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  const auto &[r, g, b, a] = prop.value();
  glUniform4f(location, r, g, b, a);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name, const EnumProperty &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  const auto num = prop.value();
  glUniform1i(location, num);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name, const FloatProperty &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  glUniform1f(location, prop.value());
}

auto MaterialProcessor::set_uniform(const std::string_view uniform_name, const int val) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  glUniform1i(location, val);
}

auto MaterialProcessor::set_texture(const std::string_view sampler_name, const gl::GLuint texture_unit) const -> void {
  set_uniform(sampler_name, texture_unit);
}
auto MaterialProcessor::set_prop(const std::string_view uniform_name, const Float2Property &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  const auto [x, y] = prop.value();
  glUniform2f(location, x, y);
};

auto MaterialProcessor::set_prop(const std::string_view uniform_name, const BoolProperty &prop) const -> void {
  set_uniform(uniform_name, (int)prop.value());
}

auto MaterialProcessor::deinit() -> void {
  gl::glDeleteProgram(program_id);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  is_initialized = false;
}

MaterialProcessor::~MaterialProcessor() { AF_ASSERT_MSG(!is_initialized, "OpenGL Leak") }

auto MaterialExecutionContext::setup_proccesors() -> void {
  EMBEDDED_DATA(mat_vertex_vert);
  {
    EMBEDDED_DATA(uniform_color_frag)
    uniform_color_processor.init(embed_data_mat_vertex_vert, embed_data_uniform_color_frag);
  }
  {
    EMBEDDED_DATA(blend_frag)
    blend_processor.init(embed_data_mat_vertex_vert, embed_data_blend_frag);
  }
  {
    EMBEDDED_DATA(blur_frag)
    blur_processor.init(embed_data_mat_vertex_vert, embed_data_blur_frag);
  }
  {
    EMBEDDED_DATA(channel_select_frag)
    channel_select_processor.init(embed_data_mat_vertex_vert, embed_data_channel_select_frag);
  }
  {
    EMBEDDED_DATA(curve_frag)
    curve_processor.init(embed_data_mat_vertex_vert, embed_data_curve_frag);
  }
}

MaterialInSocket::MaterialInSocket(UUID uuid, std::string_view ui_name, MaterialSocketType type)
    : uid(uuid), ui_name(ui_name), type(type) {}

auto MaterialInSocket::get_texture(MaterialGraph *graph) const -> GLuint {
  AF_ASSERT(link_uuid.has_value())
  const auto &link = graph->links.at(link_uuid.value());
  const auto &output = graph->get_node(link.from_node).output_socket(link.from_socket);
  return output.cache_tex.value();
}

MaterialOutSocket::MaterialOutSocket(UUID uuid, std::string_view ui_name, MaterialSocketType type)
    : uid(uuid), ui_name(ui_name), type(type) {}

auto MaterialNode::is_start_node() -> bool {
  int total_inputs = 0;
  for (auto &i : inputs) {
    total_inputs += static_cast<int>(i.link_uuid.has_value());
  }
  return inputs.empty() || total_inputs == 0;
}

auto MaterialNode::is_end_node() -> bool {
  int total_outs = 0;
  for (auto &output : outputs) {
    total_outs += (int)output.links.size();
  }
  return inputs.empty() || total_outs == 0;
}

MaterialGraphLink::MaterialGraphLink(UUID uid, UUID from_node, UUID from_socket, UUID to_node, UUID to_socket)
    : uuid(uid), from_node(from_node), from_socket(from_socket), to_node(to_node), to_socket(to_socket) {}

MaterialGraph::MaterialGraph(UUID uid) : uuid(uid) {}

void MaterialGraph::init(MaterialExecutionContext *exe_ctx) {
  auto flattened = flatten();
  std::for_each(flattened.begin(), flattened.end(), [exe_ctx](MaterialNode *node) { node->initialize(exe_ctx); });
  is_initialized = true;
}

void MaterialGraph::execute() {
  auto flattened = flatten();
  std::for_each(flattened.begin(), flattened.end(), [](MaterialNode *node) { node->execute(); });
}

auto MaterialGraph::execute_forward(UUID start_node) -> void {
  auto flattened = flatten(&get_node(start_node));
  for (auto *const node : flattened) {
    node->execute();
  }
}

void MaterialGraph::deinit() {
  auto flattened = flatten();
  std::for_each(flattened.rbegin(), flattened.rend(), [](MaterialNode *node) { node->deinitialize(); });
  is_initialized = false;
}

auto MaterialGraph::flatten(MaterialNode *start_node) -> std::vector<MaterialNode *> {
  // TODO trash code but it works
  std::set<UUID> is_visited;
  auto flattened = std::vector<MaterialNode *>();
  auto current_nodes = std::vector<MaterialNode *>();
  auto next_nodes = std::vector<MaterialNode *>();
  auto are_inputs_visited = [&is_visited, this](MaterialNode *node) -> bool {
    for (auto &in : node->inputs) {
      if (!in.link_uuid.has_value()) {
        continue;
      }
      auto f = links.at(in.link_uuid.value()).from_node;
      if (!is_visited.contains(f)) {
        return false;
      }
    }
    return true;
  };
  // Add the first layer which are the start nodes
  for (auto &node_ptr : nodes) {
    if (node_ptr.second->is_start_node()) {
      flattened.push_back(node_ptr.second.get());
      current_nodes.push_back(node_ptr.second.get());
      is_visited.insert(node_ptr.first);
    }
  }
  do {
    // add the next layer of nodes
    for (auto *node : current_nodes) {
      for (auto &out_socket : node->outputs) {
        for (auto link : out_socket.links) {
          auto &lk = links.at(link);
          if (are_inputs_visited(nodes.at(lk.to_node).get())) {
            next_nodes.push_back(nodes.at(lk.to_node).get());
          }
        }
      }
    }
    for (auto *node : next_nodes) {
      is_visited.insert(node->uuid);
    }
    current_nodes.clear();
    ranges::copy(next_nodes, std::back_inserter(flattened));
    ranges::copy(next_nodes, std::back_inserter(current_nodes));
    next_nodes.clear();
  } while (!current_nodes.empty());
  return flattened;
}

auto MaterialGraph::delete_link(UUID link_uuid) -> MaterialGraphLink {
  auto link = links.at(link_uuid);
  auto &from_node = nodes.at(link.from_node);
  auto &to_node = nodes.at(link.to_node);
  auto from_socket = std::find(from_node->outputs.begin(), from_node->outputs.end(), link.from_socket);
  auto to_socket = std::find(to_node->inputs.begin(), to_node->inputs.end(), link.to_socket);
  from_socket->links.erase(std::remove(from_socket->links.begin(), from_socket->links.end(), link_uuid),
                           from_socket->links.end());
  to_socket->link_uuid.reset();
  links.erase(link_uuid);
  return link;
}

auto MaterialGraph::create_link(UUID from_node, UUID from_socket, UUID to_node, UUID to_socket) -> UUID {
  auto link = MaterialGraphLink(generate_uuid(), from_node, from_socket, to_node, to_socket);
  links[link.uuid] = link;
  auto &f_node = nodes.at(from_node);
  auto &t_node = nodes.at(to_node);

  ranges::find_if(f_node->outputs, [&](const auto &output) {
    return output.uid == from_socket;
  })->links.push_back(link.uuid);

  ranges::find_if(t_node->inputs, [&](const auto &input) { return input.uid == to_socket; })->link_uuid = link.uuid;
  return link.uuid;
}

auto MaterialGraph::add_link(MaterialGraphLink link) -> void {
  auto lid = create_link(link.from_node, link.from_socket, link.to_node, link.to_socket);
  links.erase(lid);
  links[link.uuid] = link;
};

auto MaterialGraph::add_node(std::unique_ptr<MaterialNode> node) -> void {
  AF_ASSERT(!nodes.contains(node->uuid))
  nodes[node->uuid] = std::move(node);
}

auto MaterialGraph::delete_node(UUID node_uuid) -> std::unique_ptr<MaterialNode> {
  AF_ASSERT(nodes.contains(node_uuid))
  auto node = std::move(nodes.at(node_uuid));
  nodes.erase(node_uuid);
  return node;
}

auto MaterialGraph::add_comment(std::unique_ptr<CommentNode> comment) -> void {
  comments[comment->uuid] = std::move(comment);
}

auto MaterialGraph::delete_comment(UUID comment_uid) -> std::unique_ptr<CommentNode> {
  auto comment = std::move(comments.at(comment_uid));
  comments.erase(comment_uid);
  return comment;
}

auto MaterialGraph::dectect_cycle(UUID start_node, UUID end_node) -> bool {
  // Abstract : check if we can reach the start_node by going out of the end
  // node

  if (start_node == end_node) {
    return true;
  }
  auto next_nodes = std::queue<UUID>();
  next_nodes.push(end_node);
  while (!next_nodes.empty()) {
    auto &node = get_node(next_nodes.front());
    next_nodes.pop();
    if (node.uuid == start_node) {
      return true;
    }
    for (auto &output : node.outputs) {
      for (auto link_id : output.links) {
        next_nodes.push(links.at(link_id).to_node);
      }
    }
  }
  return false;
}

auto MaterialGraph::get_node(UUID node_uid) -> MaterialNode & { return *nodes[node_uid].get(); }

auto MaterialGraph::on_prop_change() -> void{};

const EnumItems MaterialNode::PIXEL_FORMATS = {{"Eight bit integer", "", (int)PixelFormat::eight_bit},
                                               {"Sixteen bit integer", "", (int)PixelFormat::sixteen_bit},
                                               {"Sixteen bit float", "", (int)PixelFormat::sixteen_bit_float},
                                               {"Thirty two bit integer", "", (int)PixelFormat::thirty_two_bit_float}};

const EnumItems MaterialNode::PIXEL_RATIOS = {{"Stretch", "", (int)PixelRatio::stretch},
                                              {"Square", "", (int)PixelRatio::square}};

const EnumItems MaterialNode::TILLING_MODES = {{"No tilling", "", TillingMode::no_tilling},
                                               {"Vertical Tilling", "", TillingMode::v_tilling},
                                               {"Horizontal Tilling", "", TillingMode::h_tilling},
                                               {"H & V Tilling", "", TillingMode::both_tilling}};

MaterialNode::MaterialNode(UUID uuid, std::string_view ui_name, std::vector<MaterialInSocket> inputs,
                           std::vector<MaterialOutSocket> outputs, PropsMap props, MaterialGraph *graph)
    : uuid(uuid),
      ui_name(ui_name),
      inputs(std::move(inputs)),
      outputs(std::move(outputs)),
      props(std::move(props)),
      graph(graph) {}

auto MaterialNode::initialize(MaterialExecutionContext *exe_context) -> void {
  AF_ASSERT(!is_initialized)
  exe_ctx = exe_context;
  create_output_buffers();
  is_initialized = true;
};

auto MaterialNode::deinitialize() -> void {
  AF_ASSERT(is_initialized)
  destroy_output_buffers();
  is_initialized = false;
  exe_ctx.reset();
};

auto MaterialNode::execute() -> void{};

auto MaterialNode::on_prop_change() -> void {
  if (is_initialized) {
    graph->execute_forward(uuid);
  }
}
auto MaterialNode::output_socket(UUID socket_uuid) -> MaterialOutSocket & {
  auto it = ranges::find_if(outputs, [socket_uuid](const auto &socket) { return socket.uid == socket_uuid; });
  AF_ASSERT(it != outputs.end())
  return *it;
}
auto MaterialNode::input_socket(UUID socket_uuid) -> MaterialInSocket & {
  auto it = ranges::find_if(inputs, [socket_uuid](const auto &socket) { return socket.uid == socket_uuid; });
  AF_ASSERT(it != inputs.end())
  return *it;
};

auto MaterialNode::create_output_buffers() -> void {
  for (auto &output : outputs) {
    AF_ASSERT(!output.cache_tex.has_value() && !output.cache_tex.has_value());
    GLuint texture = 0;
    GLuint framebuffer = 0;
    const auto pixel_format = common_props.get_prop<EnumProperty>("pixel_format").value();
    const GLenum internal_format = get_gl_format(output.type, (PixelFormat)(pixel_format));

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
    glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);

    const auto [width, height] = common_props.get_prop<Integer2Property>("output_size").value();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // TODO Swizzle G & B to R for grayscale

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    output.cache_tex = texture;
    output.cache_frame_buf = framebuffer;
  }
}

auto MaterialNode::destroy_output_buffers() -> void {
  for (auto &output : outputs) {
    AF_ASSERT(output.cache_tex.has_value() && output.cache_frame_buf.has_value())
    glDeleteTextures(1, &output.cache_tex.value());
    glDeleteFramebuffers(1, &output.cache_frame_buf.value());
    output.cache_frame_buf.reset();
    output.cache_tex.reset();
  }
}

auto get_gl_format(MaterialSocketType socket_type, PixelFormat pixel_format) -> gl::GLenum {
  if (socket_type == MaterialSocketType::color) {
    switch (pixel_format) {
      case PixelFormat::eight_bit:
        return GL_RGBA;
        break;
      case PixelFormat::sixteen_bit:
        return GL_RGBA16;
        break;
      case PixelFormat::sixteen_bit_float:
        return GL_RGBA16F;
        break;
      case PixelFormat::thirty_two_bit_float:
        return GL_RGBA32F;
        break;
    }
  } else if (socket_type == MaterialSocketType::grayscale) {
    switch (pixel_format) {
      case PixelFormat::eight_bit:
        return GL_R;
        break;
      case PixelFormat::sixteen_bit:
        return GL_R16;
        break;
      case PixelFormat::sixteen_bit_float:
        return GL_R16F;
        break;
      case PixelFormat::thirty_two_bit_float:
        return GL_R32F;
        break;
    }
  }
  AF_ASSERT_MSG(false, "Unkown combination")
  return GL_RGBA;
};

}  // namespace afro::core