#pragma once
#include "property/data/property.h"

namespace afro::property {
auto draw_integer_property(Property& property) -> void;
auto draw_integer2_property(Property& property) -> void;
auto draw_integer3_property(Property& property) -> void;
auto draw_integer4_property(Property& property) -> void;
auto draw_float_property(Property& property) -> void;
auto draw_float2_property(Property& property) -> void;
auto draw_float3_property(Property& property) -> void;
auto draw_float4_property(Property& property) -> void;
auto draw_bool_property(Property& property) -> void;
auto draw_string_property(Property& property) -> void;
auto draw_enum_property(Property& property) -> void;

}  // namespace afro::property
