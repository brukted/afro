/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/property.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "core/curve.h"
#include "ui/ui_utils.h"
#include "utils/log.h"

namespace afro::core {

PropertyDefinition::PropertyDefinition(const std::string_view& id,
                                       const std::string_view& name,
                                       const std::string_view& description)
    : id(id), name(name), description(description) {}

Property::Property(PropertyDefinition* propertyDefinition,
                   PropertyCallback callback)
    : property_definition(propertyDefinition), callback(std::move(callback)) {}

auto IntegerProperty::draw() -> void {
  const auto* prop_def =
      static_cast<IntegerPropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description.data());
  if (ImGui::DragInt("###drag_int", &m_value, prop_def->step, prop_def->ui_min,
                     prop_def->ui_max)) {
    m_value = std::clamp(m_value, prop_def->min, prop_def->max);
    callback();
  }
  ImGui::PopID();
}

auto IntegerProperty::value() const -> int { return m_value; }

IntegerProperty::IntegerProperty(IntegerPropertyDefinition* propertyDefinition,
                                 const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto FloatProperty::draw() -> void {
  const auto* prop_def =
      static_cast<FloatPropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description);
  if (ImGui::DragFloat("###drag_float", &m_value, prop_def->step,
                       prop_def->ui_min, prop_def->ui_max)) {
    m_value = std::clamp(m_value, prop_def->min, prop_def->max);
    callback();
  }
  ImGui::PopID();
}

auto FloatProperty::value() const -> float { return m_value; }

FloatProperty::FloatProperty(FloatPropertyDefinition* propertyDefinition,
                             const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto Float2Property::draw() -> void {
  const auto* prop_def =
      static_cast<Float2PropertyDefinition*>(property_definition);

  switch (prop_def->type) {
    case Float2PropertyType::position:
      // TODO Implement Float2Property position widget
    case Float2PropertyType::generic: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::DragFloat2("###drag_float_2", (float*)&m_value, prop_def->step,
                            prop_def->ui_min, prop_def->ui_max)) {
        m_value.x = std::clamp(m_value.x, prop_def->min, prop_def->max);
        m_value.y = std::clamp(m_value.y, prop_def->min, prop_def->max);
        callback();
      }
      ImGui::PopID();
    } break;
    case Float2PropertyType::transform_offset:
      log::core_critical("TODO implement transform offset widget");
      break;
    default:
      log::core_critical("Unknown float2 prop type");
      break;
  }
}

auto Float2Property::value() const -> FVec2 { return m_value; }

Float2Property::Float2Property(Float2PropertyDefinition* propertyDefinition,
                               const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto Float3Property::draw() -> void {
  const auto* prop_def =
      static_cast<Float3PropertyDefinition*>(property_definition);

  switch (prop_def->type) {
    case Float3PropertyType::generic: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::DragFloat3("###drag_float_3", (float*)&m_value, prop_def->step,
                            prop_def->ui_min, prop_def->ui_max)) {
        m_value.x = std::clamp(m_value.x, prop_def->min, prop_def->max);
        m_value.y = std::clamp(m_value.y, prop_def->min, prop_def->max);
        m_value.z = std::clamp(m_value.z, prop_def->min, prop_def->max);
        callback();
      }
      ImGui::PopID();
    } break;
    case Float3PropertyType::color_rgb: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::ColorEdit3("###color_edit3", (float*)&m_value)) {
        callback();
      }
      ImGui::PopID();
    } break;
  }
}

auto Float3Property::value() const -> FVec3 { return m_value; }

Float3Property::Float3Property(Float3PropertyDefinition* propertyDefinition,
                               const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto Float4Property::draw() -> void {
  const auto* prop_def =
      static_cast<Float4PropertyDefinition*>(property_definition);
  switch (prop_def->type) {
    case Float4PropertyType::generic: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::DragFloat4("###dragfloat4", (float*)&m_value, prop_def->step,
                            prop_def->ui_min, prop_def->ui_max)) {
        m_value.x = std::clamp(m_value.x, prop_def->min, prop_def->max);
        m_value.y = std::clamp(m_value.y, prop_def->min, prop_def->max);
        m_value.z = std::clamp(m_value.z, prop_def->min, prop_def->max);
        m_value.w = std::clamp(m_value.w, prop_def->min, prop_def->max);
        callback();
      }
      ImGui::PopID();
    } break;
    case Float4PropertyType::color_rgba: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::ColorEdit4("###coloredit4", (float*)&m_value)) {
        callback();
      }
      ImGui::PopID();
    } break;
  }
}

auto Float4Property::value() const -> FVec4 { return m_value; }

Float4Property::Float4Property(Float4PropertyDefinition* propertyDefinition,
                               const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto Integer2Property::draw() -> void {
  const auto* prop_def =
      static_cast<Integer2PropertyDefinition*>(property_definition);

  switch (prop_def->type) {
    case Integer2PropertyType::generic: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::DragInt2("###dergint2", (int*)&m_value, prop_def->step,
                          prop_def->ui_min, prop_def->ui_max)) {
        m_value.x = std::clamp(m_value.x, prop_def->min, prop_def->max);
        m_value.y = std::clamp(m_value.y, prop_def->min, prop_def->max);
        callback();
      }
      ImGui::PopID();

    } break;
    case Integer2PropertyType::size_pow_2: {
      ImGui::PushID(prop_def->name.data());
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      ImGui::BeginGroup();
      if (ImGui::DragInt("###x", &pow.x, prop_def->step, prop_def->ui_min,
                         prop_def->ui_max) ||
          ImGui::DragInt("###y", &pow.y, prop_def->step, prop_def->ui_min,
                         prop_def->ui_max)) {
        pow.x = std::clamp(pow.x, prop_def->min, prop_def->max);
        pow.y = std::clamp(pow.y, prop_def->min, prop_def->max);
        m_value.x = static_cast<int>(std::pow(2, pow.x));
        m_value.y = static_cast<int>(std::pow(2, pow.y));
        callback();
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      ImGui::BeginDisabled();
      ImGui::Text("%d", m_value.x);
      ImGui::Text("%d", m_value.y);
      ImGui::EndDisabled();
      ImGui::EndGroup();
      ImGui::PopID();
    } break;
  }
}

Integer2Property::Integer2Property(
    Integer2PropertyDefinition* propertyDefinition,
    const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {
  if (propertyDefinition->type == Integer2PropertyType::size_pow_2) {
    pow = propertyDefinition->default_value;
    m_value.x = static_cast<int>(std::pow(2, pow.x));
    m_value.y = static_cast<int>(std::pow(2, pow.y));
  }
}

auto Integer2Property::value() const -> IVec2 { return m_value; }

auto Integer3Property::draw() -> void {
  const auto* prop_def =
      static_cast<Integer3PropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description);
  if (ImGui::DragInt3("###drag3int", (int*)&m_value, prop_def->step,
                      prop_def->ui_min, prop_def->ui_max)) {
    m_value.x = std::clamp(m_value.x, prop_def->min, prop_def->max);
    m_value.y = std::clamp(m_value.y, prop_def->min, prop_def->max);
    m_value.z = std::clamp(m_value.z, prop_def->min, prop_def->max);
    callback();
  }
  ImGui::PopID();
}

auto Integer3Property::value() const -> IVec3 { return m_value; }

Integer3Property::Integer3Property(
    Integer3PropertyDefinition* propertyDefinition,
    const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto Integer4Property::draw() -> void {
  const auto* prop_def =
      static_cast<Integer4PropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description);
  if (ImGui::DragInt4("###drag4_slider", (int*)&m_value, prop_def->step,
                      prop_def->ui_min, prop_def->ui_max)) {
    m_value.x = std::clamp(m_value.x, prop_def->min, prop_def->max);
    m_value.y = std::clamp(m_value.y, prop_def->min, prop_def->max);
    m_value.z = std::clamp(m_value.z, prop_def->min, prop_def->max);
    m_value.w = std::clamp(m_value.w, prop_def->min, prop_def->max);
    callback();
  }
  ImGui::PopID();
}

auto Integer4Property::value() const -> IVec4 { return m_value; }

Integer4Property::Integer4Property(
    Integer4PropertyDefinition* propertyDefinition,
    const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto StringProperty::value() const -> const std::string& { return m_value; }

auto StringProperty::draw() -> void {
  const auto* prop_def =
      static_cast<StringPropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  switch (prop_def->type) {
    case StringPropertyType::text: {
      ImGui::TextUnformatted(prop_def->name.data());
      ui::tooltip(prop_def->description);
      if (ImGui::InputText("###input_text_widget", &m_value,
                           ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (m_value.size() > prop_def->max_len) {
          m_value.resize(prop_def->max_len);
        }
        callback();
      }
    } break;
    case StringPropertyType::path:
      // TODO implement
      break;
  }
  ImGui::PopID();
}
StringProperty::StringProperty(StringPropertyDefinition* propertyDefinition,
                               const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto BoolProperty::draw() -> void {
  const auto* prop_def =
      static_cast<BoolPropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description);
  if (ImGui::Checkbox("###checkbox", &m_value)) {
    callback();
  }
  ImGui::PopID();
}

auto BoolProperty::value() const -> bool { return m_value; }

BoolProperty::BoolProperty(BoolPropertyDefinition* propertyDefinition,
                           const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto EnumProperty::draw() -> void {
  const auto* prop_def =
      static_cast<EnumPropertyDefinition*>(property_definition);

  ImGui::PushID(prop_def->name.data());
  const auto& [c_name, c_des, c_num] = m_value;
  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description);
  if (ImGui::BeginCombo("###selectables", c_name.data())) {
    for (auto const& item : prop_def->items) {
      const auto& [i_name, des, num] = item;
      if (ImGui::Selectable(i_name.data(), num == c_num)) {
        m_value = item;
        callback();
      }
      ui::tooltip(des);
    }
    ImGui::EndCombo();
  }
  ImGui::PopID();
}

auto EnumProperty::value() const -> int {
  const auto [n, d, num] = m_value;
  return num;
}
EnumProperty::EnumProperty(EnumPropertyDefinition* propertyDefinition,
                           const PropertyCallback& callback)
    : Property(propertyDefinition, callback),
      m_value(propertyDefinition->default_value) {}

auto CurveProperty::draw() -> void {
  const auto* prop_def =
      static_cast<CurvePropertyDefinition*>(property_definition);

  ImGui::TextUnformatted(prop_def->name.data());
  ui::tooltip(prop_def->description);
  if (ui::curve_editor(&lum_curve, &r_curve, &g_curve, &b_curve, &a_curve)) {
    callback();
  }
}

auto CurveProperty::value(Channel channel) const -> bezier::BezierSpline {
  switch (channel) {
    case Channel::lum:
      return lum_curve;
    case Channel::r:
      return r_curve;
    case Channel::g:
      return g_curve;
    case Channel::b:
      return b_curve;
    case Channel::a:
      return a_curve;
    default: {
      log::core_warn("Unknown curve channel {}", static_cast<int>(channel));
      return lum_curve;
    }
  }
}
CurveProperty::CurveProperty(CurvePropertyDefinition* propertyDefinition,
                             const PropertyCallback& callback)
    : Property(propertyDefinition, callback) {}

PropsMap::PropsMap(std::initializer_list<value_type> props) {
  for (const value_type& prop : props) {
    auto& prop_ref = const_cast<value_type&>(prop);
    map.emplace(prop_ref.first, std::move(prop_ref.second));
  }
}

auto PropsMap::draw() -> void {
  for (auto& [id, prop] : map) {
    prop->draw();
    ImGui::Separator();
  }
}

IntegerPropertyDefinition::IntegerPropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, int defaultValue, int min, int max,
    int uiMin, int uiMax, float step)
    : PropertyDefinition(id, name, description),
      default_value(defaultValue),
      min(min),
      max(max),
      ui_min(uiMin),
      ui_max(uiMax),
      step(step) {}

FloatPropertyDefinition::FloatPropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, FloatPropertyType type,
    float defaultValue, float min, float max, float uiMin, float uiMax,
    float step)
    : PropertyDefinition(id, name, description),
      type(type),
      default_value(defaultValue),
      min(min),
      max(max),
      ui_min(uiMin),
      ui_max(uiMax),
      step(step) {}

Float2PropertyDefinition::Float2PropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, Float2PropertyType type,
    const FVec2& defaultValue, float min, float max, float uiMin, float uiMax,
    float step)
    : PropertyDefinition(id, name, description),
      type(type),
      default_value(defaultValue),
      min(min),
      max(max),
      ui_min(uiMin),
      ui_max(uiMax),
      step(step) {}

Float4PropertyDefinition::Float4PropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, const FVec4& default_value,
    Float4PropertyType type, float min, float max, float ui_min, float ui_max,
    float step)
    : PropertyDefinition(id, name, description),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {}

Float3PropertyDefinition::Float3PropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, const FVec3& default_value, float min,
    float max, float ui_min, float ui_max, float step, Float3PropertyType type)
    : PropertyDefinition(id, name, description),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {}

Integer2PropertyDefinition::Integer2PropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, const IVec2& default_value, int min,
    int max, int ui_min, int ui_max, float step, Integer2PropertyType type)
    : PropertyDefinition(id, name, description),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {}
Integer3PropertyDefinition::Integer3PropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, const IVec3& pow,
    const IVec3& default_value, int min, int max, int ui_min, int ui_max,
    float step)
    : PropertyDefinition(id, name, description),
      pow(pow),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step) {}
Integer4PropertyDefinition::Integer4PropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, const IVec4& pow,
    const IVec4& default_value, int min, int max, int ui_min, int ui_max,
    float step)
    : PropertyDefinition(id, name, description),
      pow(pow),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step) {}
StringPropertyDefinition::StringPropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, StringPropertyType type, int max_len,
    const std::string& default_value)
    : PropertyDefinition(id, name, description),
      type(type),
      max_len(max_len),
      default_value(default_value) {}
BoolPropertyDefinition::BoolPropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, bool default_value)
    : PropertyDefinition(id, name, description), default_value(default_value) {}
EnumPropertyDefinition::EnumPropertyDefinition(
    const std::string_view& id, const std::string_view& name,
    const std::string_view& description, const EnumItem& default_value,
    const EnumItems& items)
    : PropertyDefinition(id, name, description),
      default_value(default_value),
      items(items) {}
}  // namespace afro::core
