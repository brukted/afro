/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <array>
#include <functional>
#include <initializer_list>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "core/curve.h"
#include "utils/asset.h"
#include "utils/math.h"


namespace afro::core {

// TODO Support exposing and grouping

using PropertyCallback = std::function<void()>;

class Property {
 public:
  Property() = default;
  virtual auto draw() -> void = 0;
  virtual auto draw_edit() -> void = 0;

  virtual ~Property() = default;
};

class IntegerProperty : public Property {
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  int m_value;
  int default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;

 public:
  IntegerProperty(std::string_view name, std::string_view description, PropertyCallback callback, int default_value = 0,
                  int ui_min = INT_MIN, int ui_max = INT_MAX, int min = INT_MIN, int max = INT_MAX, float step = 1.0F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> int;
  ~IntegerProperty() override = default;
};

class FloatProperty : public Property {
 public:
  enum class Type : char { generic, angle, color_grayscale };

 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  float m_value;
  Type type;
  float default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;

 public:
  FloatProperty(std::string_view name, std::string_view description, PropertyCallback callback, Type type,
                float default_value = 0.0F, float ui_min = FLT_MIN, float ui_max = FLT_MAX, float min = FLT_MIN,
                float max = FLT_MAX, float step = 0.01F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> float;
  ~FloatProperty() override = default;
};

class Float2Property : public Property {
 public:
  enum class Type : char { generic, position, transform_offset };

 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  FVec2 m_value;
  FVec2 default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;
  Type type;

 public:
  Float2Property(std::string_view name, std::string_view description, Type type, PropertyCallback callback,
                 FVec2 default_value = {0.0F, 0.0F}, float ui_min = FLT_MIN, float ui_max = FLT_MAX,
                 float min = FLT_MIN, float max = FLT_MAX, float step = 0.01F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> FVec2;
  ~Float2Property() override = default;
};

class Float3Property : public Property {
 public:
  enum class Type : char { generic, color_rgb };

 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  FVec3 m_value;
  FVec3 default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;
  Type type;

 public:
  Float3Property(std::string_view name, std::string_view description, PropertyCallback callback, Type type,
                 FVec3 default_value = {0.0F, 0.0F, 0.0F}, float ui_min = FLT_MIN, float ui_max = FLT_MAX,
                 float min = FLT_MIN, float max = FLT_MAX, float step = 1.0F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> FVec3;
  ~Float3Property() override = default;
};

class Float4Property : public Property {
 public:
  enum class Type : char { generic, color_rgba };

 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  FVec4 m_value;
  FVec4 default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;
  Type type;

 public:
  Float4Property(std::string_view name, std::string_view description, PropertyCallback callback, Type type,
                 FVec4 default_value = {0.0F, 0.0F, 0.0F, 1.0F}, float ui_min = FLT_MIN, float ui_max = FLT_MAX,
                 float min = FLT_MIN, float max = FLT_MAX, float step = 1.0F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> FVec4;
  ~Float4Property() override = default;
};

class Integer2Property : public Property {
 public:
  enum class Type : char { generic, size_pow_2 };

 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  IVec2 m_value;
  IVec2 pow;
  IVec2 default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;
  Type type;

 public:
  Integer2Property(std::string_view name, std::string_view description, PropertyCallback callback, Type type,
                   IVec2 default_value = {0, 0}, int ui_min = INT_MIN, int ui_max = INT_MAX, int min = INT_MIN,
                   int max = INT_MAX, float step = 1.0F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> IVec2;
  ~Integer2Property() override = default;
};

class Integer3Property : public Property {
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  IVec3 m_value;
  IVec3 default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;

 public:
  Integer3Property(std::string_view name, std::string_view description, PropertyCallback callback,
                   IVec3 default_value = {0, 0, 0}, int ui_min = INT_MIN, int ui_max = INT_MAX, int min = INT_MIN,
                   int max = INT_MAX, float step = 1.0F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> IVec3;
  ~Integer3Property() override = default;
};

class Integer4Property : public Property {
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  IVec4 m_value;
  IVec4 default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;

 public:
  Integer4Property(std::string_view name, std::string_view description, PropertyCallback callback,
                   IVec4 default_value = {0, 0, 0, 0}, int ui_min = INT_MIN, int ui_max = INT_MAX, int min = INT_MIN,
                   int max = INT_MAX, float step = 1.0F);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> IVec4;
  ~Integer4Property() override = default;
};

class StringProperty : public Property {
 public:
  enum class Type : char { path, text };

 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  std::string m_value;
  Type type;
  int max_len;
  std::string default_value;

 public:
  StringProperty(std::string_view name, std::string_view description, PropertyCallback callback, Type type,
                 std::string_view default_value = "", int max_len = 100);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> const std::string &;
  ~StringProperty() override = default;
};

class BoolProperty : public Property {
 private:
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  bool m_value;
  bool default_value;

 public:
  BoolProperty(std::string_view name, std::string_view description, PropertyCallback callback,
               bool default_value = false);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> bool;
  ~BoolProperty() override = default;
};

// <name, description, number>
using EnumItem = std::tuple<std::string_view, std::string_view, int>;
using EnumItems = std::vector<EnumItem>;

class EnumProperty : public Property {
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  EnumItem m_value;
  EnumItem default_value;
  EnumItems items;

 public:
  EnumProperty(std::string_view name, std::string_view description, EnumItems items, int default_value,
               PropertyCallback callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> int;
  ~EnumProperty() override = default;
};

class CurveProperty : public Property {
  std::string_view name;
  PropertyCallback callback;
  std::string_view description;
  bezier::BezierSpline lum_curve;
  bezier::BezierSpline r_curve;
  bezier::BezierSpline g_curve;
  bezier::BezierSpline b_curve;
  bezier::BezierSpline a_curve;

 public:
  enum class Channel : char { lum, r, g, b, a };
  CurveProperty(std::string_view name, std::string_view description, PropertyCallback callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  /**
   * @brief Returns the curve for the given channel
   *
   * @param channel One of the channels defined in the enum
   * @return bezier::BezierSpline
   */
  [[nodiscard]] auto value(Channel channel = Channel::lum) const -> bezier::BezierSpline;
  ~CurveProperty() override = default;
};

class PropsMap {
  std::map<std::string_view, std::unique_ptr<Property>> map;
  using value_type = std::pair<const std::basic_string_view<char>, std::unique_ptr<afro::core::Property>>;

 public:
  PropsMap() = default;
  PropsMap(std::initializer_list<value_type> props);

  template <typename T>
  auto get_prop(std::string_view id) -> T & {
    auto &ptr = map.at(id);
    AF_ASSERT((dynamic_cast<T *>(ptr.get())) != nullptr)
    return *(dynamic_cast<T *>(ptr.get()));
  };

  auto draw() -> void;
};

}  // namespace afro::core
