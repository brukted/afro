#pragma once

#include <boost/signals2/signal.hpp>

#include "common/data/uuid.h"
#include "property_definition.h"
#include "property_value.h"
#include "utils/log.h"

namespace afro::property {
class Property {
 private:
  PropertyValue value;
  UUID uuid;
  PropertyDefinition property_definition;

 public:
  // Not preserved across file loads
  boost::signals2::signal<void(Property&)> on_value_changed;

  Property(const PropertyDefinition property_definition)
      : value(property_definition.default_value),
        uuid(generate_uuid()),
        property_definition(property_definition) {}

  [[nodiscard]] auto get_uuid() const -> UUID { return uuid; }

  [[nodiscard]] auto get_property_definition() const -> auto& {
    return property_definition;
  }

  template <typename T>
  auto get() -> T& {
    return std::get<T>(value);
  }

  template <typename T>
  auto set(T new_value) -> void {
    this->value = new_value;
    on_value_changed(*this);
    log::core_trace("Property {} changed signal emitted.",
                    property_definition.name);
  }

  template <typename T>
  auto operator=(T new_value) -> Property& {
    set(new_value);
    return *this;
  }
};
}  // namespace afro::property
