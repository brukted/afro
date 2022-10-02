/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <glbinding/gl43core/gl.h>

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/property.h"
#include "core/uuid.h"
#include "graph_common.h"
#include "material_engine.h"
#include "ui/icons.h"
#include "utils/translation.h"

namespace afro::core {

struct MaterialSocketDefinition;
struct MaterialNodeDefinition;

struct MaterialGraph;

enum class PixelRatio : char { stretch = 0, square };

struct MaterialInSocket {
  const UUID uid;
  const MaterialSocketDefinition *definition;
  std::optional<UUID> link_uuid;

  MaterialInSocket(const UUID uid, const MaterialSocketDefinition *definition)
      : uid(uid), definition(definition) {}

  constexpr auto operator==(UUID other_uid) const -> bool {
    return other_uid == uid;
  }
};

struct MaterialOutSocket {
  const UUID uid;
  const MaterialSocketDefinition *definition;
  std::vector<UUID> links;

  MaterialOutSocket(UUID uid, const MaterialSocketDefinition *definition);

  auto operator==(const MaterialOutSocket &rhs) const -> bool;
  auto operator!=(const MaterialOutSocket &rhs) const -> bool;

  inline auto operator==(UUID other_uid) const -> bool {
    return uid == other_uid;
  }
};
enum TillingMode : int { no_tilling = 0, v_tilling, h_tilling, both_tilling };

struct MaterialGraph;

using InvalidateBuffersCallback = std::function<void()>;
using InvalidateCacheCallback = std::function<void()>;

struct MaterialNode {
  const static EnumItems PIXEL_FORMATS;
  const static EnumItems PIXEL_RATIOS;
  const static EnumItems TILLING_MODES;

  UUID uuid;

  std::vector<MaterialInSocket> inputs;
  std::vector<MaterialOutSocket> outputs;
  const MaterialNodeDefinition *definition;

  PropsMap common_props;
  PropsMap specific_props;

  std::optional<InvalidateBuffersCallback> invalidateBuffersCallback;
  std::optional<InvalidateCacheCallback> invalidateCacheCallback;

  NodeUiInfo ui_info;
  MaterialGraph *graph = nullptr;

  MaterialNode(UUID uuid, const MaterialNodeDefinition *definition);

  auto output_socket(UUID socket_uuid) -> MaterialOutSocket &;
  auto input_socket(UUID socket_uuid) -> MaterialInSocket &;

  auto is_start_node() -> bool;
  auto is_end_node() -> bool;

  inline auto operator==(UUID other_uuid) const -> bool {
    return other_uuid == uuid;
  }
};

struct MaterialGraphLink {
  UUID uuid;

  UUID from_node;
  UUID from_socket;
  UUID to_node;
  UUID to_socket;

  MaterialGraphLink(UUID uid, UUID from_node, UUID from_socket, UUID to_node,
                    UUID to_socket);

  MaterialGraphLink() = default;

  inline auto operator==(UUID other_uid) const -> bool {
    return other_uid == uuid;
  }
};

struct MaterialGraph {
  std::string identifier{"Untitled"};

  using NodeAddedCallback = std::function<void(UUID)>;
  using NodeDeletedCallback = std::function<void(std::vector<UUID>)>;
  using LinkDeletedCallback = std::function<void(UUID)>;
  using LinkAddedCallback = std::function<void(UUID)>;

  PropsMap props;

  UUID uuid;
  std::unordered_map<UUID, std::unique_ptr<MaterialNode>> nodes;
  std::unordered_map<UUID, MaterialGraphLink> links;
  std::unordered_map<UUID, std::unique_ptr<CommentNode>> comments;

  // Callbacks
  std::optional<InvalidateBuffersCallback> invalidate_buffers_callback;
  std::optional<InvalidateCacheCallback> invalidate_cache_callback;
  std::optional<NodeAddedCallback> node_added_callback;
  std::optional<NodeDeletedCallback> node_deleted_callback;
  std::optional<LinkAddedCallback> link_added_callback;
  std::optional<LinkDeletedCallback> link_deleted_callback;

  MaterialGraph(UUID uuid);

  /// Returns the flatten graph in order of precedence of execution
  auto flatten(MaterialNode *start_node = nullptr)
      -> std::vector<MaterialNode *>;
  auto delete_link(UUID link_uuid) -> MaterialGraphLink;
  auto create_link(UUID from_node, UUID from_socket, UUID to_node,
                   UUID to_socket) -> UUID;
  auto add_link(MaterialGraphLink link) -> void;
  auto add_node(std::unique_ptr<MaterialNode> node) -> void;
  auto add_comment(std::unique_ptr<CommentNode> comment) -> void;

  auto delete_node(UUID uid) -> std::unique_ptr<MaterialNode>;
  auto delete_comment(UUID uid) -> std::unique_ptr<CommentNode>;

  /**Checks if a cycle would be created if we create a link from @a start_node
   to @a end_node**/
  auto dectect_cycle(UUID start_node, UUID end_node) -> bool;
  auto get_node(UUID node_uid) -> MaterialNode &;

  inline auto operator==(UUID other_uid) const { return other_uid == uuid; }
};

}  // namespace afro::core
