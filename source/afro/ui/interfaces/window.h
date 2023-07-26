#pragma once

#include <memory>

#include "widget.h"

namespace afro::ui {
class Window {
 public:
  virtual auto startup() -> void = 0;
  virtual auto shutdown() const -> void = 0;
  virtual auto draw() -> bool = 0;
  virtual auto add_widget(std::shared_ptr<Widget> widget) -> void = 0;
  virtual ~Window() = default;
};

}  // namespace afro::ui