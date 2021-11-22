/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "core/image_texture.h"
#include "glbinding/gl/types.h"
namespace afro::core {
struct Context;
}

namespace afro::ui {

struct TextureViewer {
  gl::GLuint texture;
  core::ImageTexture *img_texture;
};

}  // namespace afro::ui