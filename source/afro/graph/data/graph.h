/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "common/interfaces/object.h"
#include "graph_item.h"
#include "link.h"
#include "node.h"

namespace afro::graph {

class Graph : AfObject {
 protected:
  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<GraphItem>> items;
  std::vector<Link> links;

  auto add_node(std::unique_ptr<Node> node) -> void;

 public:
  Graph() = default;

  Graph(std::vector<property::Property> properties)
      : AfObject(std::move(properties)) {}

  Graph(UUID uuid, std::vector<property::Property> properties)
      : AfObject(uuid, std::move(properties)) {}

  // Nodes
  auto remove_node_by_uuid(const UUID& uuid) -> void;
  auto get_nodes() -> std::vector<std::unique_ptr<Node>>&;
  auto get_node_by_uuid(const UUID& uuid) -> Node*;

  // Links
  [[nodiscard]] auto get_links() const -> const std::vector<Link>&;
  auto add_link(Link link) -> void;
  auto remove_link_by_uuid(UUID uuid) -> void;
  auto get_link_by_uuid(UUID uuid) -> Link;
  [[nodiscard]] auto get_links_to_node(UUID uuid) -> std::vector<Link>;
  [[nodiscard]] auto get_links_from_node(UUID uuid) -> std::vector<Link>;

  // Graph Items
  auto add_item(std::unique_ptr<GraphItem> item) -> void;
  [[nodiscard]] auto get_items() const
      -> const std::vector<std::unique_ptr<GraphItem>>&;
  auto remove_item_by_uuid(UUID uuid) -> void;

  ~Graph() override = default;
};

}  // namespace afro::graph