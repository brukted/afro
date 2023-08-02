/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_processor.h"

#include "utils/assert.h"
#include "utils/embed_data.h"

using namespace gl;

EMBEDDED_DATA(mat_vertex_vert)

namespace afro::graph::material {

auto MaterialProcessor::init(std::string_view fragment_shader) -> void {
  const auto *vertex_source =
      static_cast<const char *>(embed_data_mat_vertex_vert);
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES),
               static_cast<const void *>(INDICES), GL_STATIC_DRAW);
  is_initialized = true;
}

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

auto MaterialProcessor::set_texture(std::string_view sampler_name,
                                    gl::GLint texture_unit) const -> void {
  auto location = glGetUniformLocation(program_id, sampler_name.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);
  glUniform1i(location, texture_unit);
}

auto MaterialProcessor::execute(gl::GLuint output_frame_buf, gl::GLuint tex_buf,
                                int width, int height) const -> void {
  // TODO: Add support for multiple output frame buffers
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

auto MaterialProcessor::set_prop(property::Property &prop) const -> void {
  auto location = glGetUniformLocation(
      program_id, prop.get_property_definition().id.data());
  AF_ASSERT_MSG(location != -1, "Uniform not found")
  glUseProgram(program_id);

  switch (prop.get_property_definition().value_type) {
    case property::ValueType::INTEGER:
      glUniform1i(location, prop.get<int>());
      break;
    case property::ValueType::INTEGER_2:
      glUniform2iv(location, 1, prop.get<IVec2>().data());
      break;
    case property::ValueType::INTEGER_3:
      glUniform3iv(location, 1, prop.get<IVec3>().data());
      break;
    case property::ValueType::INTEGER_4:
      glUniform4iv(location, 1, prop.get<IVec4>().data());
      break;
    case property::ValueType::FLOAT:
      glUniform1f(location, prop.get<float>());
      break;
    case property::ValueType::FLOAT_2:
      glUniform2fv(location, 1, prop.get<FVec2>().data());
      break;
    case property::ValueType::FLOAT_3:
      glUniform3fv(location, 1, prop.get<FVec3>().data());
      break;
    case property::ValueType::FLOAT_4:
      glUniform4fv(location, 1, prop.get<FVec4>().data());
      break;
    case property::ValueType::BOOLEAN:
      glUniform1i(location, static_cast<gl::GLint>(prop.get<bool>()));
      break;
    case property::ValueType::ENUM:
      glUniform1i(location, prop.get<int>());
      break;
    case property::ValueType::STRING:
    case property::ValueType::COLOR_BEZIER_CURVE:
      AF_ASSERT_MSG(false, "Unsupported property type")
      break;
  }
}

}  // namespace afro::graph::material