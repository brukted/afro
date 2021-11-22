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

#include "core/context.h"

namespace afro::ui {

enum class Icon;

auto draw_icon(Icon icon) -> void;

template <typename T, typename... Args>
auto draw_operator(const std::string_view name, core::Context *ctx, Args... args) -> bool {
  if (ImGui::Button(name.data())) {
    auto ptrs = std::unique_ptr<T>(new T(args...));
    ctx->operator_queue.emplace(std::move(ptrs));
    return true;
  }
  return false;
};

template <typename T, typename... Args>
auto draw_operator(const std::string_view name, core::Context *ctx, const Icon icon, Args... args) -> bool {
  draw_icon(icon);
  ImGui::SameLine();
  if (ImGui::Button(name.data())) {
    auto ptrs = std::unique_ptr<T>(new T(args...));
    ctx->operator_queue.emplace(std::move(ptrs));
    return true;
  }
  return false;
};

auto dark_theme() -> void;

/**
 * @brief Draws a tooltip for the last item drawn.
 *
 * @param text Text to display in the tooltip.
 * @param seconds Delay before the tooltip is displayed. By default, it is taken from preferences.
 */
auto tooltip(std::string_view text, float seconds = 1.5F) -> void;

}  // namespace afro::ui