/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/material_graph.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <queue>
#include <unordered_map>
#include <utility>

#include "core/material_node_definition.h"
#include "core/property.h"
#include "core/uuid.h"
#include "glbinding/gl/types.h"
#include "utils/asset.h"
#include "utils/embed_data.h"

using namespace gl;

namespace afro::core {

auto MaterialOutSocket::operator==(const MaterialOutSocket &rhs) const -> bool {
  return uid == rhs.uid;
}
auto MaterialOutSocket::operator!=(const MaterialOutSocket &rhs) const -> bool {
  return !(rhs == *this);
}

MaterialOutSocket::MaterialOutSocket(const UUID uid,
                                     const MaterialSocketDefinition *definition)
    : uid(uid), definition(definition) {}

auto MaterialNode::is_start_node() -> bool {
  int total_inputs = 0;
  for (auto &input : inputs) {
    total_inputs += static_cast<int>(input.link_uuid.has_value());
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

MaterialGraphLink::MaterialGraphLink(UUID uid, UUID from_node, UUID from_socket,
                                     UUID to_node, UUID to_socket)
    : uuid(uid),
      from_node(from_node),
      from_socket(from_socket),
      to_node(to_node),
      to_socket(to_socket) {}

using std::unordered_map;

auto MaterialGraph::flatten(MaterialNode *start_node)
    -> std::vector<MaterialNode *> {
  unordered_map<UUID, unsigned int> dependencies;
  auto flattened = std::vector<MaterialNode *>();
  auto queue = std::queue<MaterialNode *>();

  for (auto &node_ptr : nodes) {
    auto &node_inputs = node_ptr.second->inputs;
    dependencies[node_ptr.second->uuid] =
        std::count_if(node_inputs.begin(), node_inputs.end(),
                      [&](const MaterialInSocket &node_input) {
                        return node_input.link_uuid.has_value();
                      });
    if (dependencies[node_ptr.second->uuid] == 0) {
      queue.push(node_ptr.second.get());
    }
  }

  while (!queue.empty()) {
    auto *node = queue.front();
    queue.pop();
    flattened.push_back(node);
    for (auto &output_socket : node->outputs) {
      for (auto link_uuid : output_socket.links) {
        const auto end_node_uuid = links[link_uuid].to_node;
        dependencies[end_node_uuid] -= 1;
        if (dependencies[end_node_uuid] == 0) {
          queue.push(nodes[end_node_uuid].get());
        }
      }
    }
  }
  return flattened;
}

auto MaterialGraph::delete_link(UUID link_uuid) -> MaterialGraphLink {
  auto link = links.at(link_uuid);
  auto &from_node = nodes.at(link.from_node);
  auto &to_node = nodes.at(link.to_node);
  auto from_socket = std::find(from_node->outputs.begin(),
                               from_node->outputs.end(), link.from_socket);
  auto to_socket =
      std::find(to_node->inputs.begin(), to_node->inputs.end(), link.to_socket);
  from_socket->links.erase(std::remove(from_socket->links.begin(),
                                       from_socket->links.end(), link_uuid),
                           from_socket->links.end());
  to_socket->link_uuid.reset();
  links.erase(link_uuid);
  if (link_deleted_callback.has_value()) {
    link_deleted_callback.value()(link_uuid);
  }
  return link;
}

auto MaterialGraph::create_link(UUID from_node, UUID from_socket, UUID to_node,
                                UUID to_socket) -> UUID {
  auto link = MaterialGraphLink(generate_uuid(), from_node, from_socket,
                                to_node, to_socket);
  links[link.uuid] = link;
  auto &f_node = nodes.at(from_node);
  auto &t_node = nodes.at(to_node);

  std::find_if(f_node->outputs.begin(), f_node->outputs.end(),
               [&](const auto &output) { return output.uid == from_socket; })
      ->links.push_back(link.uuid);

  std::find_if(t_node->inputs.begin(), t_node->inputs.end(),
               [&](const auto &input) { return input.uid == to_socket; })
      ->link_uuid = link.uuid;

  if (link_added_callback.has_value()) {
    link_added_callback.value()(link.uuid);
  }
  return link.uuid;
}

auto MaterialGraph::add_link(MaterialGraphLink link) -> void {
  auto lid = create_link(link.from_node, link.from_socket, link.to_node,
                         link.to_socket);
  links.erase(lid);
  links[link.uuid] = link;
  ;
};

auto MaterialGraph::add_node(std::unique_ptr<MaterialNode> node) -> void {
  auto const node_uuid = node->uuid;
  AF_ASSERT(!nodes.contains(node_uuid))
  nodes[node_uuid] = std::move(node);
  if (node_added_callback.has_value()) node_added_callback.value()(node_uuid);
}

auto MaterialGraph::delete_node(UUID node_uuid)
    -> std::unique_ptr<MaterialNode> {
  AF_ASSERT(nodes.contains(node_uuid))
  auto node = std::move(nodes.at(node_uuid));
  nodes.erase(node_uuid);
  return node;
}

auto MaterialGraph::add_comment(std::unique_ptr<CommentNode> comment) -> void {
  comments[comment->uuid] = std::move(comment);
}

auto MaterialGraph::delete_comment(UUID comment_uid)
    -> std::unique_ptr<CommentNode> {
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

auto MaterialGraph::get_node(UUID node_uid) -> MaterialNode & {
  return *nodes[node_uid].get();
}
MaterialGraph::MaterialGraph(UUID uuid) : uuid(uuid) {}

const EnumItems MaterialNode::PIXEL_FORMATS = {
    {"Eight bit integer", "", (int)PixelFormat::eight_bit},
    {"Sixteen bit integer", "", (int)PixelFormat::sixteen_bit},
    {"Sixteen bit float", "", (int)PixelFormat::sixteen_bit_float},
    {"Thirty two bit integer", "", (int)PixelFormat::thirty_two_bit_float}};

const EnumItems MaterialNode::PIXEL_RATIOS = {
    {"Stretch", "", (int)PixelRatio::stretch},
    {"Square", "", (int)PixelRatio::square}};

const EnumItems MaterialNode::TILLING_MODES = {
    {"No tilling", "", TillingMode::no_tilling},
    {"Vertical Tilling", "", TillingMode::v_tilling},
    {"Horizontal Tilling", "", TillingMode::h_tilling},
    {"H & V Tilling", "", TillingMode::both_tilling}};

auto MaterialNode::output_socket(UUID socket_uuid) -> MaterialOutSocket & {
  auto it = std::find_if(
      outputs.begin(), outputs.end(),
      [socket_uuid](const auto &socket) { return socket.uid == socket_uuid; });
  AF_ASSERT(it != outputs.end())
  return *it;
}
auto MaterialNode::input_socket(UUID socket_uuid) -> MaterialInSocket & {
  auto it = std::find_if(
      inputs.begin(), inputs.end(),
      [socket_uuid](const auto &socket) { return socket.uid == socket_uuid; });
  AF_ASSERT(it != inputs.end())
  return *it;
}
MaterialNode::MaterialNode(UUID uuid, const MaterialNodeDefinition *definition)
    : uuid(uuid), definition(definition) {
  for (const auto &in_def : this->definition->input_socket_definitions) {
    inputs.emplace_back(generate_uuid(), &in_def);
  }

  for (const auto &out_def : this->definition->output_socket_definitions) {
    outputs.emplace_back(generate_uuid(), &out_def);
  }

  for (const auto &prop_def : definition->prop_definitions) {
  }
}
}  // namespace afro::core