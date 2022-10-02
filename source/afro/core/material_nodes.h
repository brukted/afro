/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "core/material_graph.h"
#include "core/property.h"
#include "utils/translation.h"

namespace afro::core::material_nodes {

extern std::vector<MaterialNodeDefinition> node_definitions;

auto register_node_definitions() -> void;
//
// struct TextNode : public MaterialNode {
//  static const EnumItems TEXT_ALIGN_X_ITEMS;
//  static const EnumItems TEXT_ALIGN_Y_ITEMS;
//  TextNode(UUID uuid, MaterialGraph* graph);
//  auto execute() -> void override;
//};
//
// struct UniformColorNode : MaterialNode {
//  UniformColorNode(UUID uuid, MaterialGraph* graph);
//  auto execute() -> void override;
//};
//
// struct BlendNode : public MaterialNode {
//  enum BlendMode : int {
//    add_sub = 0,
//    copy,
//    multiply,
//    screen,
//    overlay,
//    hard_light,
//    soft_light,
//    color_dodge,
//    linear_dodge,
//    color_burn,
//    linear_burn,
//    vivid_light,
//    divide,
//    subtract,
//    difference,
//    darken,
//    lighten,
//    hue,
//    saturation,
//    color,
//    luminosity,
//    pin_light,
//    hard_max,
//    exclusion
//  };
//  enum AlphaMode : int { background = 0, foreground, min, max, average, add };
//
//  static const EnumItems BLENDING_MODES;
//  static const EnumItems ALPHA_MODES;
//  BlendNode(UUID uuid, MaterialGraph* graph);
//  auto execute() -> void override;
//};
//
// struct BlurNode : public MaterialNode {
//  BlurNode(UUID uuid, MaterialGraph* graph);
//  auto execute() -> void override;
//};
//
// struct ChannelSelectNode : public MaterialNode {
// private:
//  static const EnumItems CHANNELS;
//  enum Channel : int {
//    red_1 = 0,
//    green_1,
//    blue_1,
//    alpha_1,
//    red_2,
//    green_2,
//    blue_2,
//    alpha_2
//  };
//
// public:
//  ChannelSelectNode(UUID uuid, MaterialGraph* graph);
//  auto execute() -> void override;
//};
//
// struct CurveNode : public MaterialNode {
// private:
//  static const EnumItems CHANNELS;
//
// public:
//  CurveNode(UUID uuid, MaterialGraph* graph);
//  auto execute() -> void override;
//};

}  // namespace afro::core::material_nodes