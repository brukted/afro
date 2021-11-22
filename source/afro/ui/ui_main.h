/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "ui_window.h"

namespace afro::core {
struct Context;
struct Operator;
}  // namespace afro::core

namespace afro::ui {

struct UiContext {
  const std::string_view layout_file_name = "layout.ini";
  Window main_window;
  core::Context* context = nullptr;
  bool running = true;
  core::Operator* running_operator = nullptr;

  UiContext(core::Context* context);

  static auto glfw_callback(int error, const char* description) -> void;
  auto init() -> void;
  auto deinit() const -> void;
  auto main_loop() -> void;
  auto draw() -> void;
};

}  // namespace afro::ui
