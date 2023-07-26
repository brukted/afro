#pragma once

#include <fruit/fruit.h>

#include <string_view>

#include "ui/interfaces/window.h"
struct GLFWwindow;

namespace afro::ui {
class MainWindow : public Window {
 private:
  const std::string_view layout_file_name = "layout.ini";
  GLFWwindow* glfw_window;
  std::vector<std::shared_ptr<Widget>> widgets;

  auto init_glfw() -> void;
  auto init_imgui() -> void;
  auto init_imgui_fonts() -> void;

 public:
  INJECT(MainWindow()) = default;
  auto startup() -> void override;
  auto shutdown() const -> void override;
  auto draw() -> bool override;
  auto add_widget(std::shared_ptr<Widget> widget) -> void override;
  static auto glfw_callback(int error, const char* description) -> void;
};
}  // namespace afro::ui