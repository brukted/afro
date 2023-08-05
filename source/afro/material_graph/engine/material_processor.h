/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <glbinding/gl43core/gl.h>

#include "property/data/property.h"

namespace afro::graph::material {
class MaterialProcessor {
 private:
  bool is_initialized = false;
  int texture_target_offset = 0;
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

  auto init(std::string_view fragment_shader) -> void;

  auto deinit() -> void;

  auto set_texture(std::string_view sampler_name, gl::GLint texture_unit)
      -> void;

  auto execute(gl::GLuint output_frame_buf, gl::GLuint tex_buf, int width,
               int height) -> void;

  auto set_prop(property::Property &prop) const -> void;

  MaterialProcessor(MaterialProcessor &) = delete;
  auto operator=(const MaterialProcessor &) -> MaterialProcessor & = delete;
  auto operator=(MaterialProcessor &&) -> MaterialProcessor & = default;
  MaterialProcessor(MaterialProcessor &&) = default;
  ~MaterialProcessor();
};
}  // namespace afro::graph::material
