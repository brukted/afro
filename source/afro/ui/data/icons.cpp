/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "icons.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "imgui.h"
#include "ui/utils/icons_font_awesome_5.h"
#include "utils/log.h"
namespace afro::ui {

const std::unordered_map<Icon, std::string> ICON_MAP = {
    {Icon::NONE, ""},
    {Icon::MISSING, ICON_FA_IMAGE},
    {Icon::FOLDER, ICON_FA_FOLDER},
    {Icon::MATERIAL_GRAPH, ICON_FA_PROJECT_DIAGRAM},
    {Icon::IMAGE_TEXTURE, ICON_FA_IMAGE},
    {Icon::TEXT_NODE, ICON_FA_FONT}};

auto icon_code_point(Icon icon) noexcept -> const char* {
  try {
    return ICON_MAP.at(icon).c_str();

  } catch (const std::out_of_range&) {
    return ICON_MAP.at(Icon::MISSING).c_str();
  }
}

auto icon_code_points() noexcept -> std::vector<ImWchar> {
  ImFontGlyphRangesBuilder glyph_builder;
  for (const auto& [icon, codepoint] : ICON_MAP) {
    glyph_builder.AddText(codepoint.c_str());
  }
  ImVector<ImWchar> ranges;
  glyph_builder.BuildRanges(&ranges);
  return {std::begin(ranges), std::end(ranges)};
}

}  // namespace afro::ui