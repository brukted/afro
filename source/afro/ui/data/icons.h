/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <imgui.h>

#include <vector>

namespace afro::ui {

enum class Icon : int {
  NONE = 0,
  MISSING,
  FOLDER,
  MATERIAL_GRAPH,
  FUNCTION_GRAPH,
  IMAGE_TEXTURE,
  // Atomic Nodes
  IMAGE_NODE,
  BLEND_NODE,
  BLUR_NODE,
  CHANNELS_SHUFFLE_NODE,
  CURVE_NODE,
  DIR_BLUR_NODE,
  DIR_WARP_NODE,
  DISTANCE_NODE,
  EMBOSS_NODE,
  GRADIENT_NODE,
  GRADIENT_MAP_NODE,
  GRAYSCALE_CONVERSION_NODE,
  HSL_NODE,
  INPUT_COLOR_NODE,
  INPUT_GRAYSCALE_NODE,
  LEVELS_NODE,
  NORMAL_NODE,
  OUTPUT_NODE,
  PIXEL_PROCESSOR_NODE,
  SHARPEN_NODE,
  SVG_NODE,
  TEXT_NODE,
  TRANSFORM_2D_NODE,
  UNIFORM_COLOR_NODE,
  WRAP_NODE,
  // Graph Items
  COMMENT_NODE
};

auto icon_code_point(Icon icon) noexcept -> const char *;

// Returns code points all icons in afro. For loading them from the font.
auto icon_code_points() noexcept -> std::vector<ImWchar>;

}  // namespace afro::ui
