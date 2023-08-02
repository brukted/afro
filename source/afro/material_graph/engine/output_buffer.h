/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <glbinding/gl43core/gl.h>

namespace afro::graph::material {
class OutputBuffer {
 public:
  gl::GLuint texture_id;
  gl::GLuint frame_buffer_id;

  OutputBuffer(gl::GLuint texture_id, gl::GLuint frame_buffer_id)
      : texture_id(texture_id), frame_buffer_id(frame_buffer_id) {}

  OutputBuffer() : texture_id(0), frame_buffer_id(0) {}
};
}  // namespace afro::graph::material