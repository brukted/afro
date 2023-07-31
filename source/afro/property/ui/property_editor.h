#pragma once

#include <fruit/fruit.h>

#include <functional>
#include <memory>

#include "common/interfaces/object.h"
#include "property/data/property.h"
#include "ui/interfaces/widget.h"

namespace afro::property {
class PropertyEditor : public ui::Widget {
 private:
  std::weak_ptr<AfObject> object;
  auto get_draw_function(Property &property) -> std::function<void(Property &)>;

 public:
  INJECT(PropertyEditor()) = default;
  ~PropertyEditor() override = default;

  void set_object(std::weak_ptr<AfObject> object);

  void draw() override;
};
}  // namespace afro::property
