/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <imgui.h>

#include <chrono>
#include <memory>
#include <string_view>
#include <utility>

#include "utils/preferences.h"

namespace afro::ui {

enum class Icon;

auto draw_icon(Icon icon) -> void;

/**
 * @brief Draws a tooltip for the last item drawn.
 *
 * @param text Text to display in the tooltip.
 * @param seconds Delay before the tooltip is displayed. By default, it is taken
 * from preferences.
 */
auto tooltip(std::string_view text, float delay = 1.0F) -> void;

}  // namespace afro::ui