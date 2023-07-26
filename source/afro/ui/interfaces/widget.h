#pragma once

namespace afro::ui {
class Widget {
 public:
  virtual auto draw() -> void = 0;
  virtual ~Widget() = default;
};
}  // namespace afro::ui
