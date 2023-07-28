#pragma once

#include <string>

#include "property_value.h"

namespace afro::property {

class PropertyDefinition {
 public:
  const std::string id;
  const std::string name;
  const std::string description;
  const Type type;
  const ValueType value_type;
  const ValueUnit value_unit;

  const bool is_socket;
  const bool is_editable;

  const PropertyValue default_value;
  const OptPropertyValue min_value;
  const OptPropertyValue max_value;
  const OptPropertyValue step_value;
  const std::optional<std::vector<PropertyValue>> presets;

  PropertyDefinition(
      std::string id, std::string name, std::string description, Type type,
      ValueType value_type, ValueUnit value_unit, bool is_socket,
      bool is_editable, PropertyValue default_value,
      OptPropertyValue min_value = std::nullopt,
      OptPropertyValue max_value = std::nullopt,
      OptPropertyValue step_value = std::nullopt,
      std::optional<std::vector<PropertyValue>> presets = std::nullopt)
      : id(std::move(id)),
        name(std::move(name)),
        description(std::move(description)),
        type(type),
        value_type(value_type),
        value_unit(value_unit),
        is_socket(is_socket),
        is_editable(is_editable),
        default_value(std::move(default_value)),
        min_value(std::move(min_value)),
        max_value(std::move(max_value)),
        step_value(std::move(step_value)),
        presets(std::move(presets)) {}
};
}  // namespace afro::property
