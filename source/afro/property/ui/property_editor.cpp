#include "property_editor.h"

#include <imgui.h>
#include <utils/translation.h>

#include <vector>

#include "property_widgets.h"

namespace afro::property {
void PropertyEditor::draw() {
  // Draw the main menu bar
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(translate("Editors"))) {
      ImGui::MenuItem(translate("Property Editor"), nullptr, &show);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // Draw the property editor
  if (not show) return;

  if (!ImGui::Begin(translate("Property Editor"), &show)) {
    ImGui::End();
    return;
  }

  ImGui::TextUnformatted(translate("Properties"));
  ImGui::Separator();

  if (object.expired()) {
    object.reset();
    ImGui::TextUnformatted(translate("No properties to display"));
    ImGui::End();
    return;
  }

  auto &properties = object.lock()->get_properties();
  for (auto &property : properties) {
    get_draw_function(property)(property);
  }

  ImGui::End();
}

auto PropertyEditor::get_draw_function(Property &property)
    -> std::function<void(Property &)> {
  switch (property.get_property_definition().value_type) {
    case ValueType::INTEGER:
      return draw_integer_property;
    case ValueType::INTEGER_2:
      return draw_integer2_property;
    case ValueType::INTEGER_3:
      return draw_integer3_property;
    case ValueType::INTEGER_4:
      return draw_integer4_property;
    case ValueType::FLOAT:
      return draw_float_property;
    case ValueType::FLOAT_2:
      return draw_float2_property;
    case ValueType::FLOAT_3:
      return draw_float3_property;
    case ValueType::FLOAT_4:
      return draw_float4_property;
    case ValueType::STRING:
      return draw_string_property;
    case ValueType::BOOLEAN:
      return draw_bool_property;
    case ValueType::ENUM:
      return draw_enum_property;
    case ValueType::COLOR_BEZIER_CURVE:
      return draw_curve_property;
    default:
      return [](Property &) {
        ImGui::TextUnformatted(translate("Unknown property type"));
      };
  }
}
void PropertyEditor::set_object(std::weak_ptr<AfObject> object_) {
  object = object_;
}
}  // namespace afro::property