/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "texture_viewer.h"

#include <imgui.h>

#include <algorithm>
#include <variant>

#include "core/image_texture.h"
#include "imgui_internal.h"
#include "utils/asset.h"
#include "utils/translation.h"

using namespace ImGui;
namespace afro::ui {

auto TextureViewer::draw(bool* p_open) -> void {
  if (!Begin(translate("Texture Viewer"), p_open) && viewable.has_value()) {
    End();
    return;
  }
  const auto available_size = GetCurrentWindow()->WorkRect.GetSize();
  auto len = std::min(available_size.x, available_size.y);
  const auto image_size = ImVec2{len, len};
  Image((ImTextureID)texture, image_size);
  End();
}

auto TextureViewer::set_texture(Viewable viewable_open) -> void {
  viewable = viewable_open;
  // Afro image texture
  if (std::get_if<0>(&viewable.value()) != nullptr) {
    // TODO : Implement
  }
  // OpenGL texture
  if (std::get_if<1>(&viewable.value()) != nullptr) {
    texture = std::get<1>(viewable.value());
  }
}

auto TextureViewer::reset() -> Viewable {
  AF_ASSERT(viewable.has_value())
  auto temp = viewable.value();
  viewable.reset();
  return temp;
}

auto TextureViewer::has_texture() const -> bool { return viewable.has_value(); }
}  // namespace afro::ui