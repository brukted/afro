#include "property_widgets.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <any>
#include <utility>

#include "property/data/enum_item.h"
#include "ui/utils/ui_utils.h"

namespace afro::property {
auto draw_integer_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<int>();

  static const PropertyValue default_step{1.0F};
  static const PropertyValue zero_value{0};
  static const PropertyValue max_value{std::numeric_limits<int>::max()};

  if (ImGui::DragInt(
          "###drag_int", value,
          std::get<float>(prop_def.step_value.value_or(default_step)),
          std::get<int>(prop_def.min_value.value_or(zero_value)),
          std::get<int>(prop_def.max_value.value_or(max_value)), nullptr,
          ImGuiSliderFlags_AlwaysClamp)) {
    // TODO: Add undo/redo
    property = *value;
  }
  ImGui::PopID();
}

auto draw_integer2_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<IVec2>();

  static const PropertyValue default_step{1.0F};
  static const PropertyValue zero_value{0};
  static const PropertyValue max_value{std::numeric_limits<int>::max()};

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::DragInt2(
              "###drag_int2", reinterpret_cast<int*>(value),
              std::get<float>(prop_def.step_value.value_or(default_step)),
              std::get<int>(prop_def.min_value.value_or(zero_value)),
              std::get<int>(prop_def.max_value.value_or(max_value)), nullptr,
              ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    case ValueUnit::POWER_2: {
      ImGui::BeginGroup();
      if (ImGui::DragInt("###x", &value->x,
                         std::get<float>(prop_def.step_value.value_or(
                             default_step)),  // TODO: Add undo/redo
                         std::get<int>(prop_def.min_value.value_or(zero_value)),
                         std::get<int>(prop_def.max_value.value_or(max_value)),
                         nullptr, ImGuiSliderFlags_AlwaysClamp) ||
          ImGui::DragInt("###y", &value->y,
                         std::get<float>(prop_def.step_value.value_or(
                             default_step)),  // TODO: Add undo/redo
                         std::get<int>(prop_def.min_value.value_or(zero_value)),
                         std::get<int>(prop_def.max_value.value_or(max_value)),
                         nullptr, ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      ImGui::EndGroup();
      ImGui::SameLine();

      ImGui::BeginGroup();
      ImGui::BeginDisabled();
      ImGui::Text("%d", static_cast<int>(std::pow(2, value->x)));
      ImGui::Text("%d", static_cast<int>(std::pow(2, value->y)));
      ImGui::EndDisabled();
      ImGui::EndGroup();
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for integer2 property");
  }

  ImGui::PopID();
}

auto draw_integer3_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  auto* value = &property.get<IVec3>();

  static const PropertyValue default_step{1.0F};
  static const PropertyValue zero_value{0};
  static const PropertyValue max_value{std::numeric_limits<int>::max()};

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::DragInt3(
              "###drag_int3", reinterpret_cast<int*>(value),
              std::get<float>(prop_def.step_value.value_or(default_step)),
              std::get<int>(prop_def.min_value.value_or(zero_value)),
              std::get<int>(prop_def.max_value.value_or(max_value)), nullptr,
              ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for integer3 property");
  }
  ui::tooltip(prop_def.description);
  ImGui::PopID();
}

auto draw_integer4_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<IVec4>();

  static const PropertyValue default_step{1.0F};
  static const PropertyValue zero_value{0};
  static const PropertyValue max_value{std::numeric_limits<int>::max()};

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::DragInt4(
              "###drag_int4", reinterpret_cast<int*>(value),
              std::get<float>(prop_def.step_value.value_or(default_step)),
              std::get<int>(prop_def.min_value.value_or(zero_value)),
              std::get<int>(prop_def.max_value.value_or(max_value)), nullptr,
              ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for integer4 property");
  }

  ImGui::PopID();
}

auto draw_float_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<float>();

  static const PropertyValue default_step{0.01F};
  static const PropertyValue zero_value{0.0F};
  static const PropertyValue max_value{std::numeric_limits<float>::max()};

  if (ImGui::DragFloat(
          "###drag_float", value,
          std::get<float>(prop_def.step_value.value_or(default_step)),
          std::get<float>(prop_def.min_value.value_or(zero_value)),
          std::get<float>(prop_def.max_value.value_or(max_value)), nullptr,
          ImGuiSliderFlags_AlwaysClamp)) {
    // TODO: Add undo/redo
    property = *value;
  }
  ImGui::PopID();
}

auto draw_float2_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<FVec2>();

  static const PropertyValue default_step{0.01F};
  static const PropertyValue zero_value{0.0F};
  static const PropertyValue max_value{std::numeric_limits<float>::max()};

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::DragFloat2(
              "###drag_float2", reinterpret_cast<float*>(value),
              std::get<float>(prop_def.step_value.value_or(default_step)),
              std::get<float>(prop_def.min_value.value_or(zero_value)),
              std::get<float>(prop_def.max_value.value_or(max_value)), "%f",
              ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for float2 property");
  }

  ImGui::PopID();
}

auto draw_float3_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<FVec3>();

  static const PropertyValue default_step{0.01F};
  static const PropertyValue zero_value{0.0F};
  static const PropertyValue max_value{std::numeric_limits<float>::max()};

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::DragFloat3(
              "###drag_float3", reinterpret_cast<float*>(value),
              std::get<float>(prop_def.step_value.value_or(default_step)),
              std::get<float>(prop_def.min_value.value_or(zero_value)),
              std::get<float>(prop_def.max_value.value_or(max_value)), nullptr,
              ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    case ValueUnit::COLOR: {
      if (ImGui::ColorEdit3("###color_edit3", reinterpret_cast<float*>(value),
                            ImGuiColorEditFlags_Float)) {
        //  TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for float3 property");
  }

  ImGui::PopID();
}

auto draw_float4_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<FVec4>();

  static const PropertyValue default_step{0.01F};
  static const PropertyValue zero_value{0.0F};
  static const PropertyValue max_value{std::numeric_limits<float>::max()};

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::DragFloat4(
              "###drag_float4", reinterpret_cast<float*>(value),
              std::get<float>(prop_def.step_value.value_or(default_step)),
              std::get<float>(prop_def.min_value.value_or(zero_value)),
              std::get<float>(prop_def.max_value.value_or(max_value)), nullptr,
              ImGuiSliderFlags_AlwaysClamp)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    case ValueUnit::COLOR: {
      if (ImGui::ColorEdit4("###color_edit4", reinterpret_cast<float*>(value),
                            ImGuiColorEditFlags_Float |
                                ImGuiColorEditFlags_AlphaBar |
                                ImGuiColorEditFlags_AlphaPreviewHalf)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for float4 property");
  }

  ImGui::PopID();
}

auto draw_bool_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<bool>();

  if (ImGui::Checkbox("###checkbox", value)) {
    // TODO: Add undo/redo
    property = *value;
  }
  ImGui::PopID();
}

auto draw_string_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<std::string>();

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      if (ImGui::InputText("###input_text", value)) {
        // TODO: Add undo/redo
        property = *value;
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for string property");
  }

  ImGui::PopID();
}

auto draw_enum_property(Property& property) -> void {
  const auto& prop_def = property.get_property_definition();

  ImGui::PushID(prop_def.id.data());
  ImGui::TextUnformatted(prop_def.name.data());
  ui::tooltip(prop_def.description);
  auto* value = &property.get<int>();
  auto enum_items = prop_def.presets.value_or(std::vector<PropertyValue>());

  switch (prop_def.value_unit) {
    case ValueUnit::NONE: {
      auto it = std::find_if(enum_items.begin(), enum_items.end(),
                             [&](const PropertyValue& item) {
                               const auto& enum_item = std::get<EnumItem>(item);
                               return *value == enum_item.value;
                             });
      auto selected = it != enum_items.end()
                          ? std::get<EnumItem>(*it).name.data()
                          : "Invalid Value";

      if (ImGui::BeginCombo("###combo", selected)) {
        for (const auto& item : enum_items) {
          const auto& enum_item = std::get<EnumItem>(item);

          bool is_selected = *value == enum_item.value;
          if (ImGui::Selectable(enum_item.name.data(), is_selected)) {
            property = enum_item.value;
          }
        }
        ImGui::EndCombo();
      }
      break;
    }
    default:
      throw std::runtime_error("Invalid value unit for enum property");
  }

  ImGui::PopID();
}
}  // namespace afro::property
