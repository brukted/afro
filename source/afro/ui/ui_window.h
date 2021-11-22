/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "ui/editors/material_editor.h"
#include "ui/editors/outliner.h"
#include "ui/editors/parameters_editor.h"
#include "ui/editors/preferences_editor.h"

struct GLFWwindow;
namespace afro::core {
struct Context;
}

namespace afro::ui {

struct Window {
  GLFWwindow* glfw_window;
  core::Context* context;
  Outliner outliner;
  MaterialEditor material_editor;
  PreferencesEditor preferences_editor;
  ParameterEditor parameter_editor;
  bool show_material_editor = true;
  bool show_outliner = true;
  bool show_preferences_editor = false;
  bool show_parameter_editor = true;
#ifndef NDEBUG
  bool show_undo_debug = false;
  bool show_metrics = false;
  bool show_stack_tool = false;
  bool show_style_editor = false;
  bool show_about = false;
#endif

  Window(GLFWwindow* window, core::Context* context);

  auto draw() -> void;
  auto show_exit_dialog() -> bool;

 private:
  auto main_menu_bar() -> void;
};

}  // namespace afro::ui