/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include <string_view>

#include "utils/paths.h"
#include "utils/preferences.h"

#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or
                           // multiple definition errors.

#include <GLFW/glfw3.h>
#include <glbinding/gl43core/gl.h>
#include <glbinding/glbinding.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imnodes.h>

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <vector>

#include "core/context.h"
#include "ui/icons.h"
#include "ui/ui_utils.h"
#include "ui/ui_window.h"
#include "ui_main.h"
#include "undo/operator.h"
#include "undo/undo.h"
#include "utils/embed_data.h"
#include "utils/log.h"
#include "utils/translation.h"

using afro::core::Context;
using afro::core::Operation;
using afro::core::OperatorResult;

EMBEDDED_DATA(droidsans_ttf)
EMBEDDED_DATA(fa_solid_900_ttf)

namespace afro::ui {
UiContext::UiContext(core::Context *context) : main_window(nullptr, context), context(context){};

void GLAPIENTRY open_gl_log(gl::GLenum /*unused*/, gl::GLenum /*unused*/, gl::GLuint /*unused*/, gl::GLenum severity,
                            gl::GLsizei /*unused*/, const gl::GLchar *msg, const void * /*unused*/) {
  log::core_error("OpenGL error : severity {} : {}", static_cast<unsigned int>(severity), msg);
}

auto UiContext::glfw_callback(int error, const char *description) -> void {
  log::core_error("GLFW error {} : {}", error, description);
  std::abort();
}

auto UiContext::init() -> void {
  log::core_trace("Initializing UI context");
  glfwSetErrorCallback(glfw_callback);
  if (glfwInit() == 0) {
    log::core_error("Can't initialize glfw.");
    exit(1);
  }

#if __APPLE__
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 (int)(gl::GL_TRUE));  // Required on Mac
#else
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // 3.2+
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (int)(gl::GL_TRUE));  // 3.0+ only
#endif

  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  log::core_trace("Creating main GLFW window");
  auto *window = glfwCreateWindow(1, 1, translate("Afro"), nullptr, nullptr);
  main_window.glfw_window = window;
  log::core_trace("Created main GLFW window");
  glfwMakeContextCurrent(main_window.glfw_window);
  glfwSwapInterval(1);  // Enable vsync

  // Initialize OpenGL loader
  log::core_trace("Initializing OpenGL");
  glbinding::initialize([](const char *name) { return glfwGetProcAddress(name); });
  log::core_trace("Initialized OpenGL");

#ifndef NDEBUG
  // log::core_trace("Setting OpenGL debug callback");
  // gl::glEnable(gl::GL_DEBUG_OUTPUT);
  // gl::glDebugMessageCallback(open_gl_log, nullptr);
  // log::core_trace("Set OpenGL debug callback");
#endif  //! NDEBUG

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  log::core_trace("Creating ImGui Context");
  ImGui::CreateContext();
  log::core_trace("Created ImGui Context");
  log::core_trace("Creating imnodes Context");
  imnodes::CreateContext();
  log::core_trace("Created imnodes Context");
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  static auto layout_file = (paths::config_dir() / layout_file_name).string();
  io.IniFilename = layout_file.c_str();
  if (!std::filesystem::exists(layout_file)) {
    // TODO Load default layout
    log::core_trace("Loading default layout");
    ImGui::LoadIniSettingsFromMemory("");
  } else {
    log::core_trace("ImGui will load layout from file");
  }

  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable docking
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Load fonts
  log::core_trace("Loading fonts");
  ImFontConfig config;
  config.FontDataOwnedByAtlas = false;
  constexpr float size_pixels = 16.0F;
  // Load and merge the text font
  config.MergeMode = false;
  log::core_trace("Loading DroidSans from memory");
  io.Fonts->AddFontFromMemoryTTF((void *)embed_data_droidsans_ttf, embed_data_droidsans_ttf_size, size_pixels, &config,
                                 io.Fonts->GetGlyphRangesDefault());
  log::core_trace("Loaded DroidSans from memory");
  // Load and add the embedded icon font
  config.MergeMode = true;
  const auto icon_ranges = icon_code_points();
  log::core_trace("Loading icon font from memory");
  io.Fonts->AddFontFromMemoryTTF((void *)embed_data_fa_solid_900_ttf, embed_data_fa_solid_900_ttf_size, size_pixels,
                                 &config, icon_ranges.data());
  log::core_trace("Loaded icon font from memory");
  log::core_trace("Building Fonts");
  io.Fonts->Build();
  log::core_trace("Built Fonts");

  log::core_trace("Loading style from preference file");
  auto &style = ImGui::GetStyle();
  style = Preferences::get().theme;

  // Initialize ImGui
  log::core_trace("Initializing ImGui");
  ImGui_ImplGlfw_InitForOpenGL(main_window.glfw_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  log::core_trace("Initialized ImGui");
  log::core_trace("UI Initialized");
}

auto UiContext::deinit() const -> void {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  imnodes::DestroyContext();
  ImGui::DestroyContext();
  glfwDestroyWindow(main_window.glfw_window);
  glfwTerminate();
}

auto UiContext::main_loop() -> void {
  while (running) {
    if (glfwWindowShouldClose(main_window.glfw_window) != 0) {
      if (main_window.show_exit_dialog()) {
        break;
      }
    }
    if (context->next_redo_depth != 0) {
      context->undo_stack.redo(context->next_redo_depth);
      --context->next_redo_depth;
    }
    if (context->next_undo_depth != 0) {
      context->undo_stack.undo(context->next_undo_depth);
      --context->next_undo_depth;
    }
    if (running_operator == nullptr && !context->operator_queue.empty()) {
      running_operator = context->operator_queue.front().get();
    }
    draw();
  }
  log::core_trace("Destroying UI context");
  deinit();
}

auto UiContext::draw() -> void {
  static const ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg];
  glfwPollEvents();
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // Draw running operator ui
  if (running_operator != nullptr) {
    auto result = running_operator->draw();
    switch (result) {
      case OperatorResult::CANCELED: {
        log::core_info("Operator {} canceled.", running_operator->id_name);
        running_operator = nullptr;
        context->operator_queue.pop();
      } break;
      case OperatorResult::FINISHED_UI: {
        running_operator = nullptr;
        Operation op = std::move(context->operator_queue.front());
        context->operator_queue.pop();
        try {
          op->execute();
          context->undo_stack.push_operation(std::move(op));
        } catch (const core::OperatorError &e) {
          log::core_error("Operator {} failed : {}", op->id_name, e.what());
        }
      } break;
      case OperatorResult::DRAWING_UI:
        break;
    }
  }
  main_window.draw();
  // Rendering
  ImGui::Render();
  int display_w = 0;
  int display_h = 0;
  glfwGetFramebufferSize(main_window.glfw_window, &display_w, &display_h);
  // Bind the default framebuffer
  gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);
  gl::glViewport(0, 0, display_w, display_h);
  gl::glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(main_window.glfw_window);
}
}  // namespace afro::ui