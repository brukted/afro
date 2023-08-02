/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <boost/signals2/signal.hpp>
#include <memory>

#include "common/interfaces/object.h"
#include "graph_item.h"
#include "link.h"
#include "node.h"

namespace afro::graph {

class Graph : AfObject {
 protected:
  std::vector<std::shared_ptr<Node>> nodes;
  std::vector<std::shared_ptr<GraphItem>> items;
  std::vector<Link> links;

 public:
  // Signals
  boost::signals2::signal<void(std::shared_ptr<Node>)> node_added;
  boost::signals2::signal<void(std::shared_ptr<Node>)> node_removed;
  boost::signals2::signal<void(Link)> link_added;
  boost::signals2::signal<void(Link)> link_removed;

  Graph() = default;

  Graph(std::vector<property::Property> properties)
      : AfObject(std::move(properties)) {}

  Graph(UUID uuid, std::vector<property::Property> properties)
      : AfObject(uuid, std::move(properties)) {}

  // Nodes
  auto add_node(std::shared_ptr<Node> node) -> void;
  auto remove_node_by_uuid(const UUID& uuid) -> void;
  auto get_nodes() -> std::vector<std::shared_ptr<Node>>&;
  auto get_node_by_uuid(const UUID& uuid) -> std::shared_ptr<Node>;

  // Links
  [[nodiscard]] auto get_links() const -> const std::vector<Link>&;
  auto add_link(Link link) -> void;
  auto add_links(const std::vector<Link>& links) -> void;
  auto remove_link(const Link& link) -> void;
  auto remove_links(const std::vector<Link>& links) -> void;
  [[nodiscard]] auto get_link_by_uuid(UUID uuid) -> Link;
  [[nodiscard]] auto get_links_by_uuids(const std::vector<UUID>& uuids)
      -> std::vector<Link>;
  [[nodiscard]] auto get_links_to_node(UUID uuid) -> std::vector<Link>;
  [[nodiscard]] auto get_links_from_node(UUID uuid) -> std::vector<Link>;

  // Graph Items
  auto add_item(std::shared_ptr<GraphItem> item) -> void;
  [[nodiscard]] auto get_items() const
      -> const std::vector<std::shared_ptr<GraphItem>>&;
  auto remove_item_by_uuid(UUID uuid) -> void;

  ~Graph() override = default;
};

}  // namespace afro::graph