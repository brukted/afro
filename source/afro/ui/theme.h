/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <imgui.h>

namespace afro::ui {

struct Theme {
  ImGuiStyle imgui_style;
  struct {
    struct {
      float delay;
    } tooltip{};
  } user_interface{};
};

}  // namespace afro::ui