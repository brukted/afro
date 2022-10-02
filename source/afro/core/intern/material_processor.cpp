/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/material_processor.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/gl.h>
#include <glbinding/gl/types.h>

#include <iterator>

#include "core/property.h"
#include "utils/asset.h"
#include "utils/embed_data.h"

using namespace gl;

namespace afro::core {
auto MaterialProcessor::init(std::string_view vertex_shader,
                             std::string_view fragment_shader) -> void {
  const auto *vertex_source = vertex_shader.data();
  const auto *fragment_source = fragment_shader.data();
  auto vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, nullptr);
  glCompileShader(vertex);
  auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragment_source, nullptr);
  glCompileShader(fragment);
  program_id =
      glCreateProgram();  // NOLINT(cppcoreguidelines-prefer-member-initializer)
  glAttachShader(program_id, vertex);
  glAttachShader(program_id, fragment);
  glLinkProgram(program_id);
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(VERTICES),
      VERTICES,  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
      GL_STATIC_DRAW);

  const auto stride_size = 5;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        stride_size * sizeof(GL_FLOAT), (void *)nullptr);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                        stride_size * sizeof(GL_FLOAT),
                        reinterpret_cast<void *>(3 * 4));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES,
               GL_STATIC_DRAW);
  is_initialized = true;
}

auto MaterialProcessor::execute(GLuint output_frame_buf, GLuint tex_buf,
                                int width, int height) const -> void {
  glUseProgram(program_id);
  glBindFramebuffer(gl::GLenum::GL_FRAMEBUFFER, output_frame_buf);
  glViewport(0, 0, width, height);
  glBindVertexArray(vao);
  constexpr GLsizei num_indices = 6;
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, tex_buf);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name,
                                 const Float4Property &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  const auto &[r, g, b, a] = prop.value();
  glUniform4f(location, r, g, b, a);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name,
                                 const EnumProperty &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  const auto num = prop.value();
  glUniform1i(location, num);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name,
                                 const FloatProperty &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  glUniform1f(location, prop.value());
}

auto MaterialProcessor::set_uniform(const std::string_view uniform_name,
                                    const int val) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  glUniform1i(location, val);
}

auto MaterialProcessor::set_texture(const std::string_view sampler_name,
                                    const gl::GLuint texture_unit) const
    -> void {
  set_uniform(sampler_name, texture_unit);
}

auto MaterialProcessor::set_prop(const std::string_view uniform_name,
                                 const Float2Property &prop) const -> void {
  auto location = glGetUniformLocation(program_id, uniform_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  const auto [x, y] = prop.value();
  glUniform2f(location, x, y);
};

auto MaterialProcessor::deinit() -> void {
  gl::glDeleteProgram(program_id);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  is_initialized = false;
}

MaterialProcessor::~MaterialProcessor() {
  AF_ASSERT_MSG(!is_initialized, "OpenGL Leak")
}
}  // namespace afro::core
