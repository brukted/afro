/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ui_utils.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <array>
#include <string_view>

#include "ui/data/icons.h"

using namespace ImGui;

namespace afro::ui {

auto draw_icon(Icon icon) -> void { TextUnformatted(icon_code_point(icon)); };

auto tooltip(std::string_view text, float delay) -> void {
  if (IsItemHovered() && !IsItemActive() && GImGui->HoveredIdTimer >= delay) {
    BeginTooltip();
    TextUnformatted(text.data());
    EndTooltip();
  }
}
}  // namespace afro::ui