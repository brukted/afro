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
#include "utils/translation.h"

namespace afro::core {

enum class MaterialSocketType : char { grayscale = 0, color, universal };

enum class PixelFormat : char { eight_bit = 0, sixteen_bit, sixteen_bit_float, thirty_two_bit_float };

enum class PixelRatio : char { stretch = 0, square };

struct MaterialProcessor {
  //                                            x  | y |  z  | u |  v
  // clang-format off
  static constexpr  gl::GLfloat VERTICES[] = {  1,   1,   0,   1,   1,
                                               -1,   1,   0,   0,   1,
                                                1,  -1,   0,   1,   0,
                                               -1,  -1,   0,   0,   0 };
  static constexpr int INDICES[] = {0, 1, 2, 2, 1, 3};
  // clang-format on
  bool is_initialized = false;
  gl::GLuint program_id = 0;
  gl::GLuint vbo = 0, vao = 0, ebo = 0;
  MaterialProcessor() = default;
  auto init(std::string_view vertex_shader, std::string_view fragment_shader) -> void;
  auto deinit() -> void;

  MaterialProcessor(MaterialProcessor &) = delete;
  auto operator=(const MaterialProcessor &) -> MaterialProcessor & = delete;
  auto operator=(MaterialProcessor &&) -> MaterialProcessor & = default;
  MaterialProcessor(MaterialProcessor &&) = default;
  ~MaterialProcessor();

  auto execute(gl::GLuint output_frame_buf, gl::GLuint tex_buf, int width, int height) const -> void;
  auto set_prop(const std::string_view uniform_name, const Float4Property &prop) const -> void;
  auto set_prop(const std::string_view uniform_name, const EnumProperty &prop) const -> void;
  auto set_prop(const std::string_view uniform_name, const FloatProperty &prop) const -> void;
  auto set_prop(const std::string_view uniform_name, const Float2Property &prop) const -> void;
  auto set_uniform(const std::string_view uniform_name, const int val) const -> void;
  auto set_texture(const std::string_view sampler_name, const gl::GLuint texture_unit) const -> void;
};

struct MaterialGraph;

struct MaterialInSocket {
  const UUID uid;
  const std::string_view ui_name{translate("socket")};
  const MaterialSocketType type{MaterialSocketType::color};
  std::optional<UUID> link_uuid;

  MaterialInSocket(UUID uid, std::string_view ui_name, MaterialSocketType type);

  auto get_texture(MaterialGraph *graph) const -> gl::GLuint;

  constexpr auto operator==(UUID other_uid) const -> bool { return other_uid == uid; }
};

struct MaterialOutSocket {
  const UUID uid;
  const std::string_view ui_name{translate("socket")};
  const MaterialSocketType type{MaterialSocketType::color};
  std::vector<UUID> links;
  std::optional<gl::GLuint> cache_frame_buf;
  std::optional<gl::GLuint> cache_tex;

  MaterialOutSocket(UUID uuid, std::string_view ui_name, MaterialSocketType type);

  inline auto operator==(UUID other_uid) const -> bool { return uid == other_uid; }
};

/**
 * @brief Finds the appropriate OpenGL texture format for the given socket type and pixel_format
 *
 */
auto get_gl_format(MaterialSocketType socket_type, PixelFormat pixel_format) -> gl::GLenum;

enum TillingMode : int { no_tilling = 0, v_tilling, h_tilling, both_tilling };

struct MaterialExecutionContext {
  MaterialProcessor uniform_color_processor;
  MaterialProcessor blend_processor;
  MaterialProcessor blur_processor;
  MaterialProcessor channel_select_processor;
  MaterialExecutionContext() = default;
  auto setup_proccesors() -> void;
};

struct MaterialGraph;

struct MaterialNode {
  const static EnumItems PIXEL_FORMATS;
  const static EnumItems PIXEL_RATIOS;
  const static EnumItems TILLING_MODES;

  UUID uuid;
  std::string_view ui_name;
  std::vector<MaterialInSocket> inputs;
  std::vector<MaterialOutSocket> outputs;
  NodeUiInfo ui_info;
  std::optional<MaterialExecutionContext *> exe_ctx;
  MaterialGraph *graph;
  bool is_initialized = false;
  PropsMap common_props = {
      {"output_size", std::make_unique<Integer2Property>(
                          translate("Output Size"), "description",
                          [this]() {
                            auto *execution_context = exe_ctx.value();
                            deinitialize();
                            initialize(execution_context);
                            execute();
                          },
                          Integer2Property::Type::size_pow_2, IVec2{10, 10}, 0, 12, 0, 12)},
      {"pixel_format", std::make_unique<EnumProperty>(translate("Output Format"), "description", PIXEL_FORMATS,
                                                      (int)PixelFormat::eight_bit,
                                                      [this]() {
                                                        auto *execution_context = exe_ctx.value();
                                                        deinitialize();
                                                        initialize(execution_context);
                                                        execute();
                                                      })},
      {"tilling_mode", std::make_unique<EnumProperty>(translate("Tilling Mode"), "description", TILLING_MODES,
                                                      (int)TillingMode::no_tilling, [this]() {
                                                        auto *execution_context = exe_ctx.value();
                                                        deinitialize();
                                                        initialize(execution_context);
                                                        execute();
                                                      })}};
  PropsMap props;

  MaterialNode(UUID uuid, std::string_view ui_name, std::vector<MaterialInSocket> inputs,
               std::vector<MaterialOutSocket> outputs, PropsMap props, MaterialGraph *graph);

  /**
   * @brief Create an output buffer for each output socket on the GPU.
   *
   */
  auto create_output_buffers() -> void;
  /**
   * @brief Deletes the output buffers on the GPU.
   *
   */
  auto destroy_output_buffers() -> void;
  virtual auto initialize(MaterialExecutionContext *exe_context) -> void;
  virtual auto execute() -> void;
  virtual auto deinitialize() -> void;
  virtual auto on_prop_change() -> void;
  auto output_socket(UUID socket_uuid) -> MaterialOutSocket &;
  auto input_socket(UUID socket_uuid) -> MaterialInSocket &;

  MaterialNode(MaterialNode &&) = default;
  MaterialNode(MaterialNode &) = delete;
  auto operator=(MaterialNode) -> MaterialNode & = delete;
  auto operator=(MaterialNode &&) -> MaterialNode & = delete;
  virtual ~MaterialNode() = default;

  auto is_start_node() -> bool;
  auto is_end_node() -> bool;
  inline auto operator==(UUID other_uuid) const -> bool { return other_uuid == uuid; }
};

struct MaterialGraphLink {
  UUID uuid;
  UUID from_node;
  UUID from_socket;
  UUID to_node;
  UUID to_socket;
  MaterialGraphLink(UUID uid, UUID from_node, UUID from_socket, UUID to_node, UUID to_socket);
  MaterialGraphLink() = default;
  inline auto operator==(UUID other_uid) const -> bool { return other_uid == uuid; }
};

struct MaterialGraph {
  // attributes
  std::string identifier{"Untitled"};
  int width;
  int height;
  PixelFormat image_format = PixelFormat::eight_bit;
  int seed;
  PropsMap props;
  PropsMap custom_props;

  const UUID uuid;
  bool is_initialized = false;
  std::unordered_map<UUID, std::unique_ptr<MaterialNode>> nodes;
  std::unordered_map<UUID, MaterialGraphLink> links;
  std::unordered_map<UUID, std::unique_ptr<CommentNode>> comments;

  MaterialGraph(UUID uid);
  MaterialGraph(MaterialGraph &) = delete;
  MaterialGraph(MaterialGraph &&) = default;
  auto operator=(MaterialGraph) -> MaterialGraph & = delete;
  auto operator=(MaterialGraph &&) -> MaterialGraph & = delete;
  ~MaterialGraph() = default;

  void init(MaterialExecutionContext *exe_ctx);
  void execute();
  auto execute_forward(UUID start_node) -> void;
  void deinit();
  auto on_prop_change() -> void;
  /// Returns the flatten graph in order of precedence of execution
  auto flatten(MaterialNode *start_node = nullptr) -> std::vector<MaterialNode *>;
  auto delete_link(UUID link_uuid) -> MaterialGraphLink;
  auto create_link(UUID from_node, UUID from_socket, UUID to_node, UUID to_socket) -> UUID;
  auto add_link(MaterialGraphLink link) -> void;
  auto add_node(std::unique_ptr<MaterialNode> node) -> void;
  auto add_comment(std::unique_ptr<CommentNode> comment) -> void;
  auto delete_node(UUID uid) -> std::unique_ptr<MaterialNode>;
  auto delete_comment(UUID uid) -> std::unique_ptr<CommentNode>;
  // Checks if a cycle would be created if we create a link from @a start_node
  // to @a end_node
  auto dectect_cycle(UUID start_node, UUID end_node) -> bool;
  auto get_node(UUID node_uid) -> MaterialNode &;
  inline auto operator==(UUID other_uid) const { return other_uid == uuid; }
};

}  // namespace afro::core
