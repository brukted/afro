/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "ui_window.h"

#include <imgui.h>

#include "core/context.h"
#include "debug_ui.h"
#include "icons.h"
#include "ui/editors/material_editor.h"
#include "ui/ui_utils.h"
#include "utils/translation.h"

namespace afro::ui {
Window::Window(GLFWwindow *window, core::Context *context)
    : glfw_window(window), context(context), outliner(Outliner(context)),
      material_editor(MaterialEditor(context)) {}

auto Window::draw() -> void {
  main_menu_bar();
  ImGui::DockSpaceOverViewport();
  if (show_outliner) {
    outliner.draw(&show_outliner);
  }
  if (show_material_editor) {
    material_editor.draw(&show_material_editor);
  }
  if (show_preferences_editor) {
    preferences_editor.draw(&show_preferences_editor);
  }
  if (show_parameter_editor) {
    parameter_editor.draw(&show_parameter_editor);
  }
  // Debuging Tools
#ifndef NDEBUG
  if (show_undo_debug) {
    debug_undo(context);
  }
  if (show_metrics) {
    ImGui::ShowMetricsWindow(&show_metrics);
  }
  if (show_stack_tool) {
    ImGui::ShowStackToolWindow(&show_stack_tool);
  }
  if (show_about) {
    ImGui::ShowAboutWindow(&show_about);
  }
  if (show_style_editor) {
    ImGui::Begin("Dear ImGui Style Editor", &show_style_editor);
    ImGui::ShowStyleEditor();
    ImGui::End();
  }
#endif // !NDEBUG
}

auto Window::main_menu_bar() -> void {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(translate("Editors"))) {
      ImGui::MenuItem(translate("Outliner"), nullptr, &show_outliner);
      ImGui::MenuItem(translate("Material Editor"), nullptr,
                      &show_material_editor);
      ImGui::MenuItem(translate("Preferences Editor"), nullptr,
                      &show_preferences_editor);
      ImGui::MenuItem(translate("Parameter Editor"), nullptr,
                      &show_parameter_editor);
      ImGui::EndMenu();
    }

    // Debuging Tools
#ifndef NDEBUG
    if (ImGui::BeginMenu(translate("Debug"))) {
      ImGui::MenuItem("Undo", nullptr, &show_undo_debug);
      ImGui::MenuItem("Metrics/Debugger", nullptr, &show_metrics);
      ImGui::MenuItem("Stack Tool", nullptr, &show_stack_tool);
      ImGui::MenuItem("Style Editor", nullptr, &show_style_editor);
      ImGui::MenuItem("About Dear ImGui", nullptr, &show_about);
      ImGui::EndMenu();
    }
#endif // !NDEBUG
    ImGui::EndMainMenuBar();
  }
}

auto Window::show_exit_dialog() -> bool {
  // TODO implement
  return true;
}

} // namespace afro::ui