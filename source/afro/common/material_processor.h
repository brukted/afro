/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
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
class MaterialProcessor {
 private:
  bool is_initialized = false;
  gl::GLuint program_id = 0;
  gl::GLuint vbo = 0, vao = 0, ebo = 0;
  //                                            x  | y |  z  | u |  v
  // clang-format off
  static constexpr  gl::GLfloat VERTICES[] = {  1,   1,   0,   1,   1,
                                               -1,   1,   0,   0,   1,
                                                1,  -1,   0,   1,   0,
                                               -1,  -1,   0,   0,   0 };
  static constexpr int INDICES[] = {0, 1, 2, 2, 1, 3};
  // clang-format on

 public:
  MaterialProcessor() = default;

  auto init(std::string_view vertex_shader, std::string_view fragment_shader)
      -> void;
  auto deinit() -> void;

  MaterialProcessor(MaterialProcessor &) = delete;
  auto operator=(const MaterialProcessor &) -> MaterialProcessor & = delete;
  auto operator=(MaterialProcessor &&) -> MaterialProcessor & = default;
  MaterialProcessor(MaterialProcessor &&) = default;
  ~MaterialProcessor();

  auto execute(gl::GLuint output_frame_buf, gl::GLuint tex_buf, int width,
               int height) const -> void;
  auto set_prop(std::string_view uniform_name, const Float4Property &prop) const
      -> void;
  auto set_prop(std::string_view uniform_name, const EnumProperty &prop) const
      -> void;
  auto set_prop(std::string_view uniform_name, const FloatProperty &prop) const
      -> void;
  auto set_prop(std::string_view uniform_name, const Float2Property &prop) const
      -> void;
  auto set_uniform(std::string_view uniform_name, int val) const -> void;
  auto set_texture(std::string_view sampler_name, gl::GLuint texture_unit) const
      -> void;
};
}  // namespace afro::core