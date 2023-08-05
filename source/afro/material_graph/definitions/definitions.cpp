/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "definitions.h"

#include "material_graph/data/material_node.h"
#include "material_graph/engine/material_engine.h"
#include "property/data/property.h"
#include "utils/assert.h"
#include "utils/embed_data.h"

EMBEDDED_DATA(blend_frag)
EMBEDDED_DATA(blur_frag)
EMBEDDED_DATA(channel_select_frag)
EMBEDDED_DATA(circle_frag)
EMBEDDED_DATA(curve_frag)
EMBEDDED_DATA(depth_frag)
EMBEDDED_DATA(distance_frag)
EMBEDDED_DATA(distanceprecalc_frag)
EMBEDDED_DATA(emboss_frag)
EMBEDDED_DATA(fx_frag)
EMBEDDED_DATA(gamma_frag)
EMBEDDED_DATA(gradientmap_frag)
EMBEDDED_DATA(grauscaleconv_frag)
EMBEDDED_DATA(hsl_frag)
EMBEDDED_DATA(image_basic_frag)
EMBEDDED_DATA(invert_frag)
EMBEDDED_DATA(levels_frag)
EMBEDDED_DATA(sharpen_frag)
EMBEDDED_DATA(uniform_color_frag)

namespace afro::graph::material {
using EnumPreset = std::vector<property::PropertyValue>;

const MaterialNodeDefinition solid_color_node_defintion = {
    "solid_color_node",
    "Solid Color",
    {
        {"color", "Color", "Empty desc", property::Type::INPUT,
         property::ValueType::FLOAT_4, property::ValueUnit::COLOR, false, false,
         FVec4{1.0F, 1.0F, 1.0F, 1.0F}},
        {"_output", "Output", "Empty desc", property::Type::OUTPUT,
         property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
         FVec4{0.0F, 0.0F, 0.0F, 0.0F}},
    },
    static_cast<char const*>(embed_data_uniform_color_frag),
    ui::Icon::UNIFORM_COLOR_NODE};

const EnumPreset mix_node_mode_enum_items{
    property::EnumItem{"Add Sub", 0},
    property::EnumItem{"Copy", 1},
    property::EnumItem{"Multiply", 2},
    property::EnumItem{"Screen", 3},
    property::EnumItem{"Overlay", 4},
    property::EnumItem{"Hard Light", 5},
    property::EnumItem{"Soft Light", 6},
    property::EnumItem{"Color Dodge", 7},
    property::EnumItem{"Linear Dodge", 8},
    property::EnumItem{"Color Burn", 9},
    property::EnumItem{"Linear Burn", 10},
    property::EnumItem{"Vivid Light", 11},
    property::EnumItem{"Divide", 12},
    property::EnumItem{"Subtract", 13},
    property::EnumItem{"Difference", 14},
    property::EnumItem{"Darken", 15},
    property::EnumItem{"Lighten", 16},
    property::EnumItem{"Hue", 17},
    property::EnumItem{"Saturation", 18},
    property::EnumItem{"Color", 19},
    property::EnumItem{"Luminosity", 20},
    property::EnumItem{"Linear Light", 21},
    property::EnumItem{"Pin Light", 22},
    property::EnumItem{"Hard Mix", 23},
    property::EnumItem{"Exclusion", 24},
};

const MaterialNodeDefinition mix_node_definition = {
    "mix_node",
    "Mix Node",
    {{"Foreground", "Foreground", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
      FVec4{1.0F, 1.0F, 1.0F, 1.0F}},
     {"Background", "Background", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
      FVec4{1.0F, 1.0F, 1.0F, 1.0F}},
     {"Mask", "Mask", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT, property::ValueUnit::COLOR, true, false,
      0.5F},
     {"blendMode", "Blend Mode", "Empty desc", property::Type::INPUT,
      property::ValueType::ENUM, property::ValueUnit::NONE, false, false, 0,
      std::nullopt, std::nullopt, std::nullopt, mix_node_mode_enum_items},
     {"alphaMode", "Alpha Mode", "Empty desc", property::Type::INPUT,
      property::ValueType::ENUM, property::ValueUnit::NONE, false, false, 0,
      std::nullopt, std::nullopt, std::nullopt,
      std::vector<property::PropertyValue>{
          property::EnumItem{"Background", 0},
          property::EnumItem{"Foreground", 1},
          property::EnumItem{"Min", 2},
          property::EnumItem{"Max", 3},
          property::EnumItem{"Average", 4},
          property::EnumItem{"Add", 5},
      }},
     {"alpha", "Alpha", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT, property::ValueUnit::NONE, false, false, 1.0F,
      0.0F, 1.0F},
     {"_output", "Output", "Empty desc", property::Type::OUTPUT,
      property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
      FVec4{0.0F, 0.0F, 0.0F, 0.0F}}},
    static_cast<char const*>(embed_data_blend_frag),
    ui::Icon::BLEND_NODE};

const std::vector<property::PropertyValue> channel_select_enum_items = {
    property::EnumItem{"Red 1", 0},  property::EnumItem{"Green 1", 1},
    property::EnumItem{"Blue 1", 2}, property::EnumItem{"Alpha 1", 3},
    property::EnumItem{"Red 2", 4},  property::EnumItem{"Green 2", 5},
    property::EnumItem{"Blue 2", 6}, property::EnumItem{"Alpha 2", 7},
};

const MaterialNodeDefinition channel_select_node_definition = {
    "channel_select_node",
    "Channel Select",
    {
        {"input1", "Input 1", "Empty desc", property::Type::INPUT,
         property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
         FVec4{1.0F, 1.0F, 1.0F, 1.0F}},

        {"input2", "Input 2", "Empty desc", property::Type::INPUT,
         property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
         FVec4{1.0F, 1.0F, 1.0F, 1.0F}},

        {"channel_red", "Red Channel", "Empty desc", property::Type::INPUT,
         property::ValueType::ENUM, property::ValueUnit::NONE, false, false, 0,
         std::nullopt, std::nullopt, std::nullopt, channel_select_enum_items},

        {"channel_green", "Green Channel", "Empty desc", property::Type::INPUT,
         property::ValueType::ENUM, property::ValueUnit::NONE, false, false, 1,
         std::nullopt, std::nullopt, std::nullopt, channel_select_enum_items},

        {"channel_blue", "Blue Channel", "Empty desc", property::Type::INPUT,
         property::ValueType::ENUM, property::ValueUnit::NONE, false, false, 2,
         std::nullopt, std::nullopt, std::nullopt, channel_select_enum_items},

        {"channel_alpha", "Alpha Channel", "Empty desc", property::Type::INPUT,
         property::ValueType::ENUM, property::ValueUnit::NONE, false, false, 3,
         std::nullopt, std::nullopt, std::nullopt, channel_select_enum_items},

        {"_output", "Output", "Empty desc", property::Type::OUTPUT,
         property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
         FVec4{0.0F, 0.0F, 0.0F, 0.0F}},
    },
    static_cast<char const*>(embed_data_channel_select_frag),
    ui::Icon::CHANNELS_SELECT_NODE};

const MaterialNodeDefinition circle_node_definition = {
    "circle_node",
    "Circle Node",
    {{"radius", "Radius", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT, property::ValueUnit::NONE, false, false, 0.5F,
      0.0F, 1.0F},
     {"outline", "Outline", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT, property::ValueUnit::NONE, false, false, 0.0F,
      0.0F, 1.0F},
     {"width", "Width", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT, property::ValueUnit::NONE, false, false, 0.1F,
      0.0F, 1.0F},
     {"height", "Height", "Empty desc", property::Type::INPUT,
      property::ValueType::FLOAT, property::ValueUnit::NONE, false, false, 0.1F,
      0.0F, 1.0F},
     {"_output", "Output", "Empty desc", property::Type::OUTPUT,
      property::ValueType::FLOAT_4, property::ValueUnit::COLOR, true, false,
      FVec4{0.0F, 0.0F, 0.0F, 0.0F}}},
    static_cast<char const*>(embed_data_circle_frag),
    ui::Icon::BLEND_NODE};

auto material::NodeDefinitions::get_node_definitions()
    -> std::vector<MaterialNodeDefinition> {
  return {solid_color_node_defintion, mix_node_definition,
          channel_select_node_definition, circle_node_definition};
}
}  // namespace afro::graph::material