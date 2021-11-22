#include "core/material_graph.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <utility>

using namespace afro::core;
using namespace std;

TEST(MaterialGraphTest, add_link) {
  MaterialGraph graph = MaterialGraph(generate_uuid());
  auto node1 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node2 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto uid1 = node1->uuid;
  auto sock_1 = node1->outputs[0].uid;
  auto uid2 = node2->uuid;
  auto sock_2 = node2->inputs[0].uid;
  graph.add_node(move(node1));
  graph.add_node(move(node2));
  auto lk_id = graph.create_link(uid1, sock_1, uid2, sock_2);
  EXPECT_TRUE(!graph.nodes[uid1]->outputs[0].links.empty());
  EXPECT_TRUE(graph.nodes[uid1]->outputs[0].links[0] == lk_id);
  EXPECT_TRUE(graph.nodes[uid2]->inputs[0].link.has_value());
  EXPECT_TRUE(graph.nodes[uid2]->inputs[0].link.value() == lk_id);
  auto link = graph.links.at(lk_id);
  EXPECT_TRUE(link.from_node == uid1 && link.from_socket == sock_1 && link.to_node == uid2 &&
              link.to_socket == sock_2);
}

TEST(MaterialGraphTest, delete_link) {
  MaterialGraph graph = MaterialGraph(generate_uuid());
  auto node1 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node2 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto uid1 = node1->uuid;
  auto sock_1 = node1->outputs[0].uid;
  auto uid2 = node2->uuid;
  auto sock_2 = node2->inputs[0].uid;
  graph.add_node(move(node1));
  graph.add_node(move(node2));
  auto lk_id = graph.create_link(uid1, sock_1, uid2, sock_2);
  graph.delete_link(lk_id);
  EXPECT_TRUE(graph.nodes[uid1]->outputs[0].links.empty());
  EXPECT_FALSE(graph.nodes[uid2]->inputs[0].link.has_value());
  EXPECT_TRUE(graph.links.empty());
}

TEST(MaterialGraphTest, delete_node) {
  MaterialGraph graph = MaterialGraph(generate_uuid());
  auto node1 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node2 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto uid1 = node1->uuid;
  auto sock_1 = node1->outputs[0].uid;
  auto uid2 = node2->uuid;
  auto sock_2 = node2->inputs[0].uid;
  graph.add_node(move(node1));
  graph.add_node(move(node2));
  auto lk_id = graph.create_link(uid1, sock_1, uid2, sock_2);
  graph.delete_node(uid2);
  EXPECT_TRUE(graph.nodes[uid1]->outputs[0].links.empty());
  EXPECT_TRUE(graph.links.empty());
  EXPECT_TRUE(graph.nodes.size() == 1);
}

TEST(MaterialGraphTest, flatten_linear) {
  MaterialGraph graph = MaterialGraph(generate_uuid());
  auto node1 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node2 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node2", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node3 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node3", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node4 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node4", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto nodes = vector<MaterialNode*>{node4.get(), node3.get(), node2.get(), node1.get()};
  graph.add_node(move(node1));
  graph.add_node(move(node2));
  graph.add_node(move(node3));
  graph.add_node(move(node4));
  graph.create_link(nodes[0]->uuid, nodes[0]->outputs[0].uuid, nodes[1]->uid,
                    nodes[1]->inputs[0].uid);
  graph.create_link(nodes[1]->uuid, nodes[1]->outputs[0].uuid, nodes[2]->uid,
                    nodes[2]->inputs[0].uid);
  graph.create_link(nodes[2]->uuid, nodes[2]->outputs[0].uuid, nodes[3]->uid,
                    nodes[3]->inputs[0].uid);
  auto flattened = graph.flatten();
  auto node_ids = vector<UUID>();
  for_each(flattened.begin(), flattened.end(),
           [&node_ids](MaterialNode* node) { node_ids.push_back(node->uuid); });
  EXPECT_TRUE(node_ids[0] == nodes[0]->uuid && node_ids[1] == nodes[1]->uuid &&
              node_ids[2] == nodes[2]->uuid && node_ids[3] == nodes[3]->uuid);
}

TEST(MaterialGraphTest, flatten_complex) {
  MaterialGraph graph = MaterialGraph(generate_uuid());
  auto node1 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node2 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node2",
      {MaterialInSocket(generate_uuid(), "socket1"), MaterialInSocket(generate_uuid(), "socket2")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node3 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node3", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node4 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node4", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto nodes = vector<MaterialNode*>{node1.get(), node2.get(), node3.get(), node4.get()};
  graph.add_node(move(node1));
  graph.add_node(move(node2));
  graph.add_node(move(node3));
  graph.add_node(move(node4));
  graph.create_link(nodes[3]->uuid, nodes[3]->outputs[0].uuid, nodes[2]->uid,
                    nodes[2]->inputs[0].uid);  // node 4 to node 3
  graph.create_link(nodes[3]->uuid, nodes[3]->outputs[0].uid, nodes[1]->uuid,
                    nodes[1]->inputs[0].uid);  // node 4 to node 2
  graph.create_link(nodes[2]->uuid, nodes[2]->outputs[0].uid, nodes[1]->uuid,
                    nodes[1]->inputs[1].uid);  // node 3 to node 2
  graph.create_link(nodes[1]->uuid, nodes[1]->outputs[0].uid, nodes[0]->uuid,
                    nodes[0]->inputs[0].uid);  // node 2 to node 1
  auto flatten = graph.flatten();
  auto flatten_ids = vector<UUID>();
  for (auto p : flatten) {
    flatten_ids.push_back(p->uuid);
  }
  EXPECT_TRUE(flatten_ids[0] == nodes[3]->uuid && flatten_ids[1] == nodes[2]->uuid &&
              flatten_ids[2] == nodes[1]->uuid && flatten_ids[3] == nodes[0]->uuid);
}

TEST(MaterialGraphTest, dectect_cycle) {
  MaterialGraph graph = MaterialGraph(generate_uuid());
  /// Create 4 dummy nodes
  auto node1 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node1", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node2 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node2", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node3 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node3", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));
  auto node4 = unique_ptr<MaterialNode>(new MaterialNode(
      generate_uuid(), "Dummy Node4", {MaterialInSocket(generate_uuid(), "socket1")},
      {MaterialOutSocket(generate_uuid(), "socket out 1")}, nullptr));

  auto nodes = vector<MaterialNode*>{nullptr, node1.get(), node2.get(), node3.get(), node4.get()};
  graph.add_node(move(node1));
  graph.add_node(move(node2));
  graph.add_node(move(node3));
  graph.add_node(move(node4));
  /// Create links from node 4 to 3 to 2 to 1
  graph.create_link(nodes[4]->uuid, nodes[4]->outputs[0].uid, nodes[3]->uuid,
                    nodes[3]->inputs[0].uid);  // node 4 to node 3
  graph.create_link(nodes[3]->uuid, nodes[3]->outputs[0].uid, nodes[2]->uuid,
                    nodes[2]->inputs[0].uid);  // node 3 to node 2
  graph.create_link(nodes[2]->uuid, nodes[2]->outputs[0].uid, nodes[1]->uuid,
                    nodes[1]->inputs[0].uid);  // node 2 to node 1
  auto has_cycle = graph.dectect_cycle(nodes[2]->uuid, nodes[4]->uuid);
  EXPECT_TRUE(has_cycle);
}