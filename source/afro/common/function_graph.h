/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <core/uuid.h>

#include <cstdint>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "core/property.h"
#include "graph_common.h"
#include "utils/translation.h"

namespace afro::core {

enum class FunctionSocketType {
  none,  // for development only
  Boolean,
  Float,
  Float2,
  Float3,
  Float4,
  Integer,
  Integer2,
  Integer3,
  Integer4
};

struct FunctionInSocket {
  const UUID uid;
  const std::string_view ui_name{translate("socket")};
  const FunctionSocketType type{FunctionSocketType::none};
  std::optional<UUID> link;

  FunctionInSocket(UUID uid, std::string_view ui_name);

  inline auto operator==(UUID other_uid) const -> bool { return other_uid == uid; }
};

struct FunctionOutSocket {
  const UUID uid;
  const std::string_view ui_name{translate("socket")};
  const FunctionSocketType type{FunctionSocketType::none};
  std::vector<UUID> links;

  FunctionOutSocket(UUID uid, std::string_view ui_name);

  inline auto operator==(UUID other_uid) const -> bool { return uid == other_uid; }
};

struct FunctionNode {
  const UUID uid;
  const std::string_view ui_name{translate("function node")};
  NodeUiInfo ui_info;
  std::vector<FunctionInSocket> inputs;
  std::vector<FunctionOutSocket> outputs;
  PropsMap props;
  FunctionNode(UUID uid, std::string_view ui_name, std::initializer_list<FunctionInSocket> inputs,
               std::initializer_list<FunctionOutSocket> outputs);
  auto is_start_node() -> bool;
  auto is_end_node() -> bool;
  virtual ~FunctionNode() = default;
  inline auto operator==(UUID other_uid) const -> bool { return other_uid == uid; }
};

struct FunctionGraphLink {
  UUID uid;
  UUID from_node;
  UUID from_socket;
  UUID to_node;
  UUID to_socket;
  FunctionGraphLink(UUID uid, UUID from_node, UUID from_socket, UUID to_node, UUID to_socket);
  FunctionGraphLink() = default;
  inline auto operator==(UUID other_uid) const -> bool { return other_uid == uid; }
};

struct FunctionGraph {
  // attributes
  std::string identifier{};

  const UUID uid;
  std::unordered_map<UUID, std::unique_ptr<FunctionNode>> nodes;
  std::unordered_map<UUID, FunctionGraphLink> links;

  FunctionGraph(UUID uid);

  void init();
  void execute();
  void deinit();
  /// Returns the flatten graph in order of precedence of execution
  auto flatten() -> std::vector<FunctionNode *>;
  auto delete_link(UUID uid) -> FunctionGraphLink;
  auto create_link(UUID from_node, UUID from_socket, UUID to_node, UUID to_socket) -> UUID;
  auto add_node(std::unique_ptr<FunctionNode> node) -> void;
  auto delete_node(UUID uid) -> std::unique_ptr<FunctionNode>;
  auto dectect_cycle(UUID start_node, UUID end_node) -> bool;
  auto get_node(UUID node_uid) -> FunctionNode &;
  auto compile_to_gl() -> std::string;
  inline auto operator==(UUID other_uid) const { return other_uid == uid; }
  ~FunctionGraph() = default;
};

}  // namespace afro::core
