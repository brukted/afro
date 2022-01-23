/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <optional>
#include <variant>

#include "core/image_texture.h"
#include "glbinding/gl/types.h"

namespace afro::core {
struct Context;
}

namespace afro::ui {

class TextureViewer {
 public:
  using Viewable = std::variant<core::ImageTexture*, gl::GLuint>;

 private:
  gl::GLuint texture = 0;
  std::optional<Viewable> viewable;

 public:
  TextureViewer() = default;
  auto draw(bool* p_open) -> void;
  auto set_texture(Viewable viewable) -> void;
  auto reset() -> Viewable;
  [[nodiscard]] auto has_texture() const -> bool;
};

}  // namespace afro::ui