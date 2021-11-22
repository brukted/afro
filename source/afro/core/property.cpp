/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "property.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <xutility>

#include "ui/ui_utils.h"

namespace afro::core {

IntegerProperty::IntegerProperty(std::string_view name, std::string_view description, PropertyCallback callback,
                                 int default_value, int ui_min, int ui_max, int min, int max, float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step) {}

auto IntegerProperty::draw() -> void {
  if (ImGui::DragInt(name.data(), &m_value, step, ui_min, ui_max)) {
    m_value = std::clamp(m_value, min, max);
    callback();
  }
  ui::tooltip(description);
}

auto IntegerProperty::value() const -> int { return m_value; }

FloatProperty::FloatProperty(std::string_view name, std::string_view description, PropertyCallback callback, Type type,
                             float default_value, float ui_min, float ui_max, float min, float max, float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      type(type),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step) {}

auto FloatProperty::draw() -> void {
  if (ImGui::DragFloat(name.data(), &m_value, step, ui_min, ui_max)) {
    m_value = std::clamp(m_value, min, max);
    callback();
  }
  ui::tooltip(description);
}

auto FloatProperty::value() const -> float { return m_value; }

Float2Property::Float2Property(std::string_view name, std::string_view description, Type type,
                               PropertyCallback callback, FVec2 default_value, float ui_min, float ui_max, float min,
                               float max, float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {}

auto Float2Property::draw() -> void {
  switch (type) {
    case Type::generic: {
      if (ImGui::DragFloat2(name.data(), (float*)&m_value, step, ui_min, ui_max)) {
        m_value.x = std::clamp(m_value.x, min, max);
        m_value.y = std::clamp(m_value.y, min, max);
        callback();
      }
      ui::tooltip(description);

    } break;
    case Type::position:
    case Type::transform_offset:
      // TODO implement
      break;
  }
}

auto Float2Property::value() const -> FVec2 { return m_value; }

Float3Property::Float3Property(std::string_view name, std::string_view description, PropertyCallback callback,
                               Type type, FVec3 default_value, float ui_min, float ui_max, float min, float max,
                               float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {}

auto Float3Property::draw() -> void {
  switch (type) {
    case Type::generic: {
      if (ImGui::DragFloat3(name.data(), (float*)&m_value, step, ui_min, ui_max)) {
        m_value.x = std::clamp(m_value.x, min, max);
        m_value.y = std::clamp(m_value.y, min, max);
        m_value.z = std::clamp(m_value.z, min, max);
        callback();
      }
      ui::tooltip(description);

    } break;
    case Type::color_rgb: {
      if (ImGui::ColorEdit3(name.data(), (float*)&m_value)) {
        callback();
      }
      ui::tooltip(description);
    } break;
  }
}

auto Float3Property::value() const -> FVec3 { return m_value; }

Float4Property::Float4Property(std::string_view name, std::string_view description, PropertyCallback callback,
                               Type type, FVec4 default_value, float ui_min, float ui_max, float min, float max,
                               float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {}

auto Float4Property::draw() -> void {
  switch (type) {
    case Type::generic: {
      if (ImGui::DragFloat4(name.data(), (float*)&m_value, step, ui_min, ui_max)) {
        m_value.x = std::clamp(m_value.x, min, max);
        m_value.y = std::clamp(m_value.y, min, max);
        m_value.z = std::clamp(m_value.z, min, max);
        m_value.w = std::clamp(m_value.w, min, max);
        callback();
      }
      ui::tooltip(description);
    } break;
    case Type::color_rgba: {
      if (ImGui::ColorEdit4(name.data(), (float*)&m_value)) {
        callback();
      }
      ui::tooltip(description);
    } break;
  }
}

auto Float4Property::value() const -> FVec4 { return m_value; }

Integer2Property::Integer2Property(std::string_view name, std::string_view description, PropertyCallback callback,
                                   Type type, IVec2 default_value, int ui_min, int ui_max, int min, int max, float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step),
      type(type) {
  if (type == Type::size_pow_2) {
    pow = default_value;
    m_value.x = static_cast<int>(std::pow(2, pow.x));
    m_value.y = static_cast<int>(std::pow(2, pow.y));
  }
}

auto Integer2Property::draw() -> void {
  switch (type) {
    case Type::generic: {
      if (ImGui::DragInt2(name.data(), (int*)&m_value, step, ui_min, ui_max)) {
        m_value.x = std::clamp(m_value.x, min, max);
        m_value.y = std::clamp(m_value.y, min, max);
        callback();
      }
      ui::tooltip(description);

    } break;
    case Type::size_pow_2: {
      ImGui::BeginGroup();
      if (ImGui::DragInt("###x", &pow.x, step, ui_min, ui_max) ||
          ImGui::DragInt("###y", &pow.y, step, ui_min, ui_max)) {
        pow.x = std::clamp(pow.x, min, max);
        pow.y = std::clamp(pow.y, min, max);
        m_value.x = static_cast<int>(std::pow(2, pow.x));
        m_value.y = static_cast<int>(std::pow(2, pow.y));
        callback();
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      ImGui::BeginDisabled();
      ImGui::DragInt("###x_prev", &m_value.x, step, ui_min, ui_max);
      ImGui::DragInt("###y_prev", &m_value.y, step, ui_min, ui_max);
      ImGui::EndDisabled();
      ImGui::EndGroup();
      ui::tooltip(description);
    } break;
  }
}

auto Integer2Property::value() const -> IVec2 { return m_value; }

Integer3Property::Integer3Property(std::string_view name, std::string_view description, PropertyCallback callback,
                                   IVec3 default_value, int ui_min, int ui_max, int min, int max, float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step) {}

auto Integer3Property::draw() -> void {
  if (ImGui::DragInt3(name.data(), (int*)&m_value, step, ui_min, ui_max)) {
    m_value.x = std::clamp(m_value.x, min, max);
    m_value.y = std::clamp(m_value.y, min, max);
    m_value.z = std::clamp(m_value.z, min, max);
    callback();
  }
  ui::tooltip(description);
}

auto Integer3Property::value() const -> IVec3 { return m_value; }

Integer4Property::Integer4Property(std::string_view name, std::string_view description, PropertyCallback callback,
                                   IVec4 default_value, int ui_min, int ui_max, int min, int max, float step)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value),
      min(min),
      max(max),
      ui_min(ui_min),
      ui_max(ui_max),
      step(step) {}

auto Integer4Property::draw() -> void {
  if (ImGui::DragInt4(name.data(), (int*)&m_value, step, ui_min, ui_max)) {
    m_value.x = std::clamp(m_value.x, min, max);
    m_value.y = std::clamp(m_value.y, min, max);
    m_value.z = std::clamp(m_value.z, min, max);
    m_value.w = std::clamp(m_value.w, min, max);
    callback();
  }
  ui::tooltip(description);
}

auto Integer4Property::value() const -> IVec4 { return m_value; }

StringProperty::StringProperty(std::string_view name, std::string_view description, PropertyCallback callback,
                               Type type, std::string_view default_value, int max_len)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      type(type),
      max_len(max_len),
      default_value(default_value) {}

auto StringProperty::value() const -> const std::string& { return m_value; }

auto StringProperty::draw() -> void {
  switch (type) {
    case Type::text: {
      if (ImGui::InputText(name.data(), &m_value, ImGuiInputTextFlags_EnterReturnsTrue)) {
        m_value.resize(max_len);
        callback();
      }
      ui::tooltip(description);
    } break;
    case Type::path:
      // TODO implement
      break;
  }
}

BoolProperty::BoolProperty(std::string_view name, std::string_view description, PropertyCallback callback,
                           bool default_value)
    : name(name),
      callback(std::move(callback)),
      description(description),
      m_value(default_value),
      default_value(default_value) {}

auto BoolProperty::draw() -> void {
  if (ImGui::Checkbox(name.data(), &m_value)) {
    callback();
  }
  ui::tooltip(description);
}

auto BoolProperty::value() const -> bool { return m_value; }

EnumProperty::EnumProperty(std::string_view name, std::string_view description, EnumItems items, int default_value,
                           PropertyCallback callback)
    : name(name), callback(std::move(callback)), description(description), items(std::move(items)) {
  const auto default_item = std::ranges::find_if(this->items, [default_value](const auto& item) {
    const auto& [name, des, num] = item;
    return num == default_value;
  });
  this->default_value = *default_item;
  m_value = this->default_value;
}

auto EnumProperty::draw() -> void {
  const auto& [c_name, c_des, c_num] = m_value;
  if (ImGui::BeginCombo(name.data(), c_name.data())) {
    for (auto const& item : items) {
      const auto& [i_name, des, num] = item;
      if (ImGui::Selectable(i_name.data(), num == c_num)) {
        m_value = item;
        callback();
      }
      ui::tooltip(des, 2.0F);
    }
    ImGui::EndCombo();
    ui::tooltip(description);
  }
}

auto EnumProperty::value() const -> int {
  const auto [n, d, num] = m_value;
  return num;
}

PropsMap::PropsMap(std::initializer_list<value_type> props) {
  for (const value_type& prop : props) {
    auto& prop_ref = const_cast<value_type&>(prop);
    map.emplace(prop_ref.first, std::move(prop_ref.second));
  }
}

auto PropsMap::draw() -> void {
  for (auto& [id, prop] : map) {
    prop->draw();
  }
}

}  // namespace afro::core
