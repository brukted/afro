/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <array>
#include <cfloat>
#include <functional>
#include <initializer_list>
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

struct PropertyDefinition {
  std::string_view id;
  std::string_view name;
  std::string_view description;
  PropertyDefinition(const std::string_view &id, const std::string_view &name,
                     const std::string_view &description);
};

class Property {
 protected:
  PropertyDefinition *property_definition;

 public:
  PropertyCallback callback;

  Property() = default;
  Property(PropertyDefinition *propertyDefinition, PropertyCallback callback);
  virtual auto draw() -> void = 0;
  virtual auto draw_edit() -> void = 0;

  virtual ~Property() = default;
};

struct IntegerPropertyDefinition : PropertyDefinition {
  int default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;

  IntegerPropertyDefinition(const std::string_view &id,
                            const std::string_view &name,
                            const std::string_view &description,
                            int defaultValue, int min, int max, int uiMin,
                            int uiMax, float step);
};

class IntegerProperty : public Property {
 private:
  int m_value{};

 public:
  IntegerProperty(IntegerPropertyDefinition *propertyDefinition,
                  const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> int;
};

enum class FloatPropertyType : char { generic, angle, color_grayscale };

struct FloatPropertyDefinition : PropertyDefinition {
  FloatPropertyType type;
  float default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;

  FloatPropertyDefinition(const std::string_view &id,
                          const std::string_view &name,
                          const std::string_view &description,
                          FloatPropertyType type, float defaultValue, float min,
                          float max, float uiMin, float uiMax, float step);
};

class FloatProperty : public Property {
 private:
  float m_value;

 public:
  FloatProperty(FloatPropertyDefinition *propertyDefinition,
                const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> float;
};

enum class Float2PropertyType : char { generic, position, transform_offset };

struct Float2PropertyDefinition : PropertyDefinition {
  Float2PropertyType type;
  FVec2 default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;

  Float2PropertyDefinition(const std::string_view &id,
                           const std::string_view &name,
                           const std::string_view &description,
                           Float2PropertyType type, const FVec2 &defaultValue,
                           float min, float max, float uiMin, float uiMax,
                           float step);
};

class Float2Property : public Property {
 private:
  FVec2 m_value;

 public:
  Float2Property(Float2PropertyDefinition *propertyDefinition,
                 const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> FVec2;
};

enum class Float3PropertyType : char { generic, color_rgb };

struct Float3PropertyDefinition : PropertyDefinition {
  FVec3 default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;
  Float3PropertyType type;

  Float3PropertyDefinition(const std::string_view &id,
                           const std::string_view &name,
                           const std::string_view &description,
                           const FVec3 &default_value, float min, float max,
                           float ui_min, float ui_max, float step,
                           Float3PropertyType type);
};

class Float3Property : public Property {
 private:
  FVec3 m_value;

 public:
  Float3Property(Float3PropertyDefinition *propertyDefinition,
                 const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> FVec3;
};

enum class Float4PropertyType : char { generic, color_rgba };

struct Float4PropertyDefinition : PropertyDefinition {
  FVec4 default_value;
  float min;
  float max;
  float ui_min;
  float ui_max;
  float step;
  Float4PropertyType type;

  Float4PropertyDefinition(const std::string_view &id,
                           const std::string_view &name,
                           const std::string_view &description,
                           const FVec4 &default_value, Float4PropertyType type,
                           float min = FLT_MIN, float max = FLT_MAX,
                           float ui_min = FLT_MIN, float ui_max = FLT_MAX,
                           float step = 0.1F);
};

class Float4Property : public Property {
 private:
  FVec4 m_value;

 public:
  Float4Property(Float4PropertyDefinition *propertyDefinition,
                 const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> FVec4;
  ~Float4Property() override = default;
};
enum class Integer2PropertyType : char { generic, size_pow_2 };

struct Integer2PropertyDefinition : PropertyDefinition {
  IVec2 default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;
  Integer2PropertyType type;

  Integer2PropertyDefinition(const std::string_view &id,
                             const std::string_view &name,
                             const std::string_view &description,
                             const IVec2 &default_value, int min, int max,
                             int ui_min, int ui_max, float step,
                             Integer2PropertyType type);
};

class Integer2Property : public Property {
 private:
  IVec2 pow;
  IVec2 m_value;

 public:
  Integer2Property(Integer2PropertyDefinition *propertyDefinition,
                   const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> IVec2;
  ~Integer2Property() override = default;
};

struct Integer3PropertyDefinition : PropertyDefinition {
  IVec3 pow;
  IVec3 default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;

  Integer3PropertyDefinition(const std::string_view &id,
                             const std::string_view &name,
                             const std::string_view &description,
                             const IVec3 &pow, const IVec3 &default_value,
                             int min, int max, int ui_min, int ui_max,
                             float step);
};

class Integer3Property : public Property {
 private:
  IVec3 m_value;

 public:
  Integer3Property(Integer3PropertyDefinition *propertyDefinition,
                   const PropertyCallback &callback);

  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> IVec3;
  ~Integer3Property() override = default;
};

struct Integer4PropertyDefinition : PropertyDefinition {
  IVec4 pow;
  IVec4 default_value;
  int min;
  int max;
  int ui_min;
  int ui_max;
  float step;

  Integer4PropertyDefinition(const std::string_view &id,
                             const std::string_view &name,
                             const std::string_view &description,
                             const IVec4 &pow, const IVec4 &default_value,
                             int min, int max, int ui_min, int ui_max,
                             float step);
};

class Integer4Property : public Property {
 private:
  IVec4 m_value;

 public:
  Integer4Property(Integer4PropertyDefinition *propertyDefinition,
                   const PropertyCallback &callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> IVec4;
  ~Integer4Property() override = default;
};

enum class StringPropertyType : char { path, text };

struct StringPropertyDefinition : PropertyDefinition {
  StringPropertyType type;
  int max_len;
  std::string default_value;

  StringPropertyDefinition(const std::string_view &id,
                           const std::string_view &name,
                           const std::string_view &description,
                           StringPropertyType type, int max_len,
                           const std::string &default_value);
};

class StringProperty : public Property {
 private:
  std::string m_value;

 public:
  StringProperty(StringPropertyDefinition *propertyDefinition,
                 const PropertyCallback &callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> const std::string &;
  ~StringProperty() override = default;
};

struct BoolPropertyDefinition : PropertyDefinition {
  bool default_value;

  BoolPropertyDefinition(const std::string_view &id,
                         const std::string_view &name,
                         const std::string_view &description,
                         bool default_value);
};

class BoolProperty : public Property {
 private:
  bool m_value;

 public:
  BoolProperty(BoolPropertyDefinition *propertyDefinition,
               const PropertyCallback &callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> bool;
  ~BoolProperty() override = default;
};

// <name, description, number>
using EnumItem = std::tuple<std::string_view, std::string_view, int>;
using EnumItems = std::vector<EnumItem>;

struct EnumPropertyDefinition : PropertyDefinition {
  EnumItem default_value;
  EnumItems items;

  EnumPropertyDefinition(const std::string_view &id,
                         const std::string_view &name,
                         const std::string_view &description,
                         const EnumItem &default_value, const EnumItems &items);
};

class EnumProperty : public Property {
 private:
  EnumItem m_value;

 public:
  EnumProperty(EnumPropertyDefinition *propertyDefinition,
               const PropertyCallback &callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  [[nodiscard]] auto value() const -> int;
  ~EnumProperty() override = default;
};

struct CurvePropertyDefinition : PropertyDefinition {};

class CurveProperty : public Property {
 private:
  bezier::BezierSpline lum_curve;
  bezier::BezierSpline r_curve;
  bezier::BezierSpline g_curve;
  bezier::BezierSpline b_curve;
  bezier::BezierSpline a_curve;

 public:
  enum class Channel : char { lum, r, g, b, a };
  CurveProperty(CurvePropertyDefinition *propertyDefinition,
                const PropertyCallback &callback);
  auto draw() -> void override;
  auto draw_edit() -> void override{};
  /**
   * @brief Returns the curve for the given channel
   *
   * @param channel One of the channels defined in the enum
   * @return bezier::BezierSpline
   */
  [[nodiscard]] auto value(Channel channel = Channel::lum) const
      -> bezier::BezierSpline;
  ~CurveProperty() override = default;
};

class PropsMap {
  std::map<std::string_view, std::unique_ptr<Property>> map;
  using value_type = std::pair<const std::basic_string_view<char>,
                               std::unique_ptr<afro::core::Property>>;

 public:
  PropsMap() = default;
  PropsMap(std::initializer_list<value_type> props);

  template <typename T>
  auto get_prop(std::string_view id) -> T & {
    auto &ptr = map.at(id);
    AF_ASSERT((dynamic_cast<T *>(ptr.get())) != nullptr)
    return *(dynamic_cast<T *>(ptr.get()));
  }

  auto draw() -> void;
};

}  // namespace afro::core
