/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <glbinding/gl43core/gl.h>
#include <sys/dtrace.h>

#include <map>

namespace afro::core {
struct OutputSocketBuffer {
  gl::GLuint texture_id;
  gl::GLuint frame_buffer_id;

  OutputSocketBuffer(gl::GLuint texture_id, gl::GLuint frame_buffer_id)
      : texture_id(texture_id), frame_buffer_id(frame_buffer_id) {}

  OutputSocketBuffer() : texture_id(0), frame_buffer_id(0) {}
};

enum class MaterialSocketType : char { grayscale = 0, color, universal };

enum class PixelFormat : char {
  eight_bit = 0,
  sixteen_bit,
  sixteen_bit_float,
  thirty_two_bit_float
};
}  // namespace afro::core