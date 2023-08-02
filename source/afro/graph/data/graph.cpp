/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "graph.h"

#include <algorithm>
#include <ranges>

namespace views = std::ranges::views;

namespace afro::graph {

auto Graph::add_node(std::shared_ptr<Node> node) -> void {
  nodes.push_back(node);
  node_added(std::move(node));
}

auto Graph::remove_node_by_uuid(const UUID& uuid) -> void {
  auto iter = std::remove_if(nodes.begin(), nodes.end(),
                             [&uuid](const std::shared_ptr<Node>& node) {
                               return node->get_uuid() == uuid;
                             });
  std::shared_ptr<Node> node = *iter;
  nodes.erase(iter);
  node_removed(std::move(node));
  // TODO: Remove links
}

auto Graph::get_nodes() -> std::vector<std::shared_ptr<Node>>& { return nodes; }

auto Graph::get_node_by_uuid(const UUID& uuid) -> std::shared_ptr<Node> {
  auto it = std::find_if(nodes.begin(), nodes.end(),
                         [&uuid](const std::shared_ptr<Node>& node) {
                           return node->get_uuid() == uuid;
                         });
  return (it != nodes.end()) ? *it : nullptr;
}

auto Graph::get_links() const -> const std::vector<Link>& {
  return this->links;
}

auto Graph::add_link(Link link) -> void {
  this->links.push_back(link);
  link_added(link);
}

auto Graph::remove_link(const Link& link) -> void {
  auto it = std::remove(links.begin(), links.end(), link.get_uuid());
  links.erase(it);
  link_removed(link);
}

auto Graph::get_link_by_uuid(const UUID uuid) -> Link {
  auto it = std::find(links.begin(), links.end(), uuid);

  if (it != links.end()) {
    return *it;
  }

  throw std::runtime_error("Link not found");
}

auto Graph::get_links_to_node(const UUID uuid) -> std::vector<Link> {
  auto res = links | views::filter(
                         [&](Link link) { return link.get_to_node() == uuid; });
  return {res.begin(), res.end()};
}

auto Graph::get_links_from_node(const UUID uuid) -> std::vector<Link> {
  auto res = links | views::filter([&](Link link) {
               return link.get_from_node() == uuid;
             });
  return {res.begin(), res.end()};
}

auto Graph::add_item(std::shared_ptr<GraphItem> item) -> void {
  items.push_back(std::move(item));
}

auto Graph::get_items() const
    -> const std::vector<std::shared_ptr<GraphItem>>& {
  return items;
}

auto Graph::remove_item_by_uuid(UUID uuid) -> void {
  auto it = std::remove_if(items.begin(), items.end(), [&](const auto& item) {
    return item->get_uuid() == uuid;
  });
  if (it != items.end()) {
    items.erase(it);
  }

  throw std::runtime_error("Item not found");
}

auto Graph::add_links(const std::vector<Link>& links) -> void {
  this->links.insert(this->links.end(), links.begin(), links.end());
}
auto Graph::remove_links(const std::vector<Link>& links) -> void {
  for (const auto& link : links) {
    this->remove_link(link);
  }
}
auto Graph::get_links_by_uuids(const std::vector<UUID>& uuids)
    -> std::vector<Link> {
  std::vector<Link> res;
  for (const auto& uuid : uuids) {
    res.push_back(this->get_link_by_uuid(uuid));
  }
  return res;
}

}  // namespace afro::graph