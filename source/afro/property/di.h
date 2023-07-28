#pragma once

#include <fruit/fruit.h>

#include "ui/property_editor.h"

namespace afro::property {
auto get_property_component() -> fruit::Component<PropertyEditor> {
  return fruit::createComponent();
}
}  // namespace afro::property