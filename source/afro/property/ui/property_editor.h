#pragma once

#include <fruit/fruit.h>

#include <functional>
#include <memory>

#include "property/data/property.h"
#include "ui/interfaces/widget.h"

namespace afro::property {
class PropertyEditor : public ui::Widget {
 private:
  std::shared_ptr<std::vector<std::shared_ptr<Property>>> properties;
  auto get_draw_function(Property &property) -> std::function<void(Property &)>;

 public:
  INJECT(PropertyEditor()) = default;
  ~PropertyEditor() override = default;

  void set_properties(
      std::shared_ptr<std::vector<std::shared_ptr<Property>>> properties) {
    this->properties = properties;
  }

  void draw() override;
};
}  // namespace afro::property
