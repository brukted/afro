#pragma once

#include <boost/signals2/signal.hpp>

#include "property_definition.h"
#include "property_value.h"

namespace afro::property {
class Property {
 private:
  PropertyValue value;

 public:
  const PropertyDefinition property_definition;
  boost::signals2::signal<void(Property&)> on_value_changed;

  Property(const PropertyDefinition property_definition)
      : value(property_definition.default_value),
        property_definition(property_definition) {}

  template <typename T>
  auto get() -> T& {
    return std::get<T>(value);
  }

  template <typename T>
  auto set(T new_value) -> void {
    this->value = new_value;
    on_value_changed(*this);
  }

  template <typename T>
  auto operator=(T new_value) -> Property& {
    set(new_value);
    return *this;
  }
};
}  // namespace afro::property
