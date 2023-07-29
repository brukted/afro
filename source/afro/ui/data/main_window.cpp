/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or
                           // multiple definition errors.

#include "main_window.h"

#include <GLFW/glfw3.h>
#include <glbinding/gl43core/gl.h>
#include <glbinding/glbinding.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "icons.h"
#include "imnodes/imnodes.h"
#include "utils/embed_data.h"
#include "utils/log.h"
#include "utils/paths.h"
#include "utils/preferences.h"
#include "utils/translation.h"

EMBEDDED_DATA(droidsans_ttf)
EMBEDDED_DATA(fa_solid_900_ttf)

namespace afro::ui {

void GLAPIENTRY open_gl_log(gl::GLenum /*unused*/, gl::GLenum /*unused*/,
                            gl::GLuint /*unused*/, gl::GLenum severity,
                            gl::GLsizei /*unused*/, const gl::GLchar *msg,
                            const void * /*unused*/) {
  log::core_error("OpenGL error : severity {} : {}",
                  static_cast<unsigned int>(severity), msg);
}

auto MainWindow::glfw_callback(int error, const char *description) -> void {
  log::core_error("GLFW error {} : {}", error, description);
  std::abort();
}

auto MainWindow::init_glfw() -> void {
  log::core_trace("initializing glfw");
  glfwSetErrorCallback(glfw_callback);
  if (glfwInit() == 0) {
    log::core_error("glfw failed to initialize");
    exit(1);
  }

#if __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 (int)(gl::GL_TRUE));  // Required on Mac
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // 3.2+
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (int)(gl::GL_TRUE));  // 3.0+ only
#endif

  log::core_trace("creating glfw window");
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfw_window = glfwCreateWindow(1, 1, translate("Afro"), nullptr, nullptr);
  glfwMakeContextCurrent(glfw_window);
  glfwSwapInterval(1);  // Enable vsync

  // Initialize OpenGL loader
  log::core_trace("initializing OpenGL");
  glbinding::initialize(
      [](const char *name) { return glfwGetProcAddress(name); });

#ifndef NDEBUG
  // log::core_trace("setting OpenGL debug callback");
  // gl::glEnable(gl::GL_DEBUG_OUTPUT);
  // gl::glDebugMessageCallback(open_gl_log, nullptr);
#endif  //! NDEBUG
}

auto MainWindow::init_imgui() -> void {
  log::core_trace("creating imgui context");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImNodes::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;

  log::core_trace("setting up imgui style");

  static auto layout_file = (paths::config_dir() / layout_file_name).string();
  io.IniFilename = layout_file.c_str();

  if (!std::filesystem::exists(layout_file)) {
    // TODO Load default layout
    log::core_trace("loading default layout");
    ImGui::LoadIniSettingsFromMemory("");
  } else {
    log::core_trace("loading layout from file");
    ImGui::LoadIniSettingsFromDisk(layout_file.c_str());
  }

  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable docking
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  log::core_trace("loading style from preference");
  auto &style = ImGui::GetStyle();
  style = Preferences::get().theme;

  // Initialize ImGui
  log::core_trace("initializing ImGui");
  ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
  ImGui_ImplOpenGL3_Init("#version 150");
}

auto MainWindow::init_imgui_fonts() -> void {
  constexpr float size_pixels = 16.0F;
  log::core_trace("loading fonts");

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImFontConfig config;
  config.FontDataOwnedByAtlas = false;

  // Load embedded text font
  config.MergeMode = false;
  log::core_trace("loading text font from memory");
  io.Fonts->AddFontFromMemoryTTF((void *)embed_data_droidsans_ttf,
                                 embed_data_droidsans_ttf_size, size_pixels,
                                 &config, io.Fonts->GetGlyphRangesDefault());

  // Append icon font
  config.MergeMode = true;
  const auto icon_ranges = icon_code_points();
  log::core_trace("loading icon font from memory");
  io.Fonts->AddFontFromMemoryTTF((void *)embed_data_fa_solid_900_ttf,
                                 embed_data_fa_solid_900_ttf_size, size_pixels,
                                 &config, icon_ranges.data());

  io.Fonts->Build();
  log::core_trace("fonts loaded");
}

auto MainWindow::startup() -> void {
  init_glfw();
  init_imgui();
  init_imgui_fonts();
}

auto MainWindow::shutdown() const -> void {
  log::core_trace("shutting down main window");
  ImGui::SaveIniSettingsToDisk(
      (paths::config_dir() / layout_file_name).string().c_str());
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImNodes::DestroyContext();
  ImGui::DestroyContext();
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
}

auto MainWindow::add_widget(std::shared_ptr<Widget> widget) -> void {
  widgets.push_back(std::move(widget));
}

auto MainWindow::draw() -> bool {
  glfwPollEvents();

  if (glfwWindowShouldClose(glfw_window) != 0) {
    return false;
  }

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport();
  // TODO: Draw current operator

  // Draw widgets
  for (auto &widget : widgets) {
    widget->draw();
  }

  // Rendering
  ImGui::Render();
  int display_w = 0;
  int display_h = 0;
  glfwGetFramebufferSize(glfw_window, &display_w, &display_h);

  // Bind the default frame-buffer
  gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);
  gl::glViewport(0, 0, display_w, display_h);
  static const ImVec4 clear_color =
      ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg];
  gl::glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(glfw_window);
  return true;
}
}  // namespace afro::ui