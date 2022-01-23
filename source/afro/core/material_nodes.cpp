/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "material_nodes.h"

#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/imageio.h>
#include <glbinding/gl43core/gl.h>

#include <limits>
#include <memory>
#include <vector>

#include "OpenImageIO/span.h"
#include "core/material_graph.h"
#include "core/property.h"
#include "core/uuid.h"
#include "glbinding/gl/enum.h"
#include "glbinding/gl/functions-patches.h"
#include "utils/asset.h"
#include "utils/log.h"
#include "utils/translation.h"

namespace afro::core::material_nodes {

using std::make_unique;
using namespace gl;

const EnumItems TextNode::TEXT_ALIGN_X_ITEMS = {
    {"Left", "", (int)OIIO::ImageBufAlgo::TextAlignX::Left},
    {"Center", "", (int)OIIO::ImageBufAlgo::TextAlignX::Center},
    {"Right", "", (int)OIIO::ImageBufAlgo::TextAlignX::Right},
};

const EnumItems TextNode::TEXT_ALIGN_Y_ITEMS = {
    {"Baseline", "", (int)OIIO::ImageBufAlgo::TextAlignY::Baseline},
    {"Top", "", (int)OIIO::ImageBufAlgo::TextAlignY::Top},
    {"Center", "", (int)OIIO::ImageBufAlgo::TextAlignY::Center},
    {"Bottom", "", (int)OIIO::ImageBufAlgo::TextAlignY::Bottom},
};

TextNode::TextNode(UUID uuid, MaterialGraph *graph)
    : MaterialNode(
          uuid, translate("Text", "material node"),
          // Inputs
          {},
          // Outputs
          {MaterialOutSocket(generate_uuid(), translate("Output"), MaterialSocketType::color)},
          // Properties
          {{"text", make_unique<StringProperty>(
                        translate("Text"), translate("Text to render"), [this] { on_prop_change(); },
                        StringProperty::Type::text)},
           {"font_name",
            make_unique<StringProperty>(
                translate("Font name"), "", [this] { on_prop_change(); }, StringProperty::Type::text, "Arial")},
           {"font_size", make_unique<FloatProperty>(
                             translate("Font Size"), translate("Size of the font relative to the width of the image."),
                             [this] { on_prop_change(); }, FloatProperty::Type::generic)},
           {"background_color",
            make_unique<Float4Property>(
                translate("Background Color"), "", [this] { on_prop_change(); }, Float4Property::Type::color_rgba)},
           {"text_col",
            make_unique<Float4Property>(
                translate("Text Color"), "", [this] { on_prop_change(); }, Float4Property::Type::color_rgba)},
           {"text_position", make_unique<Float2Property>(translate("Position"), "", Float2Property::Type::position,
                                                         [this] { on_prop_change(); })},
           {"text_align_x",
            make_unique<EnumProperty>(translate("Align X"), "", TEXT_ALIGN_X_ITEMS,
                                      int(OIIO::ImageBufAlgo::TextAlignX::Left), [this] { on_prop_change(); })},
           {"text_align_y",
            make_unique<EnumProperty>(translate("Align Y"), "", TEXT_ALIGN_Y_ITEMS,
                                      int(OIIO::ImageBufAlgo::TextAlignY::Baseline), [this] { on_prop_change(); })}},
          graph) {}

void TextNode::execute() {
  AF_ASSERT(is_initialized && outputs[0].cache_tex.has_value())
  const auto [x_pos, y_pos] = props.get_prop<Float2Property>("text_position").value();
  const auto font_size = props.get_prop<FloatProperty>("font_size").value();
  const auto &font_name = props.get_prop<StringProperty>("font_name").value();
  const auto color = props.get_prop<Float4Property>("text_col").value();
  const auto &bg_color = props.get_prop<Float4Property>("background_color").value();
  const auto &text = props.get_prop<StringProperty>("text").value();
  const auto &[width, height] = common_props.get_prop<Integer2Property>("output_size").value();
  const auto pixel_format = common_props.get_prop<EnumProperty>("pixel_format").value();
  const auto align_x = (OIIO::ImageBufAlgo::TextAlignX)(props.get_prop<EnumProperty>("text_align_x").value());
  const auto align_y = (OIIO::ImageBufAlgo::TextAlignY)(props.get_prop<EnumProperty>("text_align_y").value());

  auto type = OIIO::TypeDesc(OIIO::TypeDesc::UINT8);
  gl::GLenum gl_type = GL_UNSIGNED_BYTE;

  switch ((PixelFormat)pixel_format) {
    case PixelFormat::eight_bit:
      type = OIIO::TypeDesc(OIIO::TypeDesc::UINT8);
      break;
    case PixelFormat::sixteen_bit: {
      type = OIIO::TypeDesc(OIIO::TypeDesc::UINT16);
      gl_type = GL_UNSIGNED_SHORT;
    } break;
    case PixelFormat::sixteen_bit_float: {
      type = OIIO::TypeDesc(OIIO::TypeDesc::HALF);
      gl_type = GL_HALF_FLOAT;
    } break;
    case PixelFormat::thirty_two_bit_float: {
      type = OIIO::TypeDesc(OIIO::TypeDesc::FLOAT);
      gl_type = GL_FLOAT;
    } break;
  }

  OIIO::ImageSpec image_spec{width, height, 4, type};
  OIIO::ImageBuf buffer{image_spec, OIIO::InitializePixels::No};

  if (!OIIO::ImageBufAlgo::fill(buffer, (float *)&bg_color)) {
    log::core_critical("Can't fill buffer in text node. OIIO error : {}", buffer.geterror());
  }

  if (!OIIO::ImageBufAlgo::render_text(buffer, static_cast<int>(x_pos * width), static_cast<int>(y_pos * height), text,
                                       static_cast<int>(font_size * width), font_name,
                                       OIIO::cspan<float>{(float *)&color, 4}, align_x, align_y)) {
    log::core_critical("Can't render text in text node. OIIO error : {}", buffer.geterror());
  }
  gl::glBindTexture(gl::GL_TEXTURE_2D, outputs[0].cache_tex.value());
  void *pixels = buffer.pixeladdr(0, 0);

  glTexSubImage2D(gl::GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, gl_type, pixels);
}

UniformColorNode::UniformColorNode(UUID uuid, MaterialGraph *graph)
    : MaterialNode(
          uuid, translate("Uniform Color"),
          // Inputs
          {},
          // Outputs
          {MaterialOutSocket(generate_uuid(), translate("Output"), MaterialSocketType::color)},
          // Props
          {{"color", make_unique<Float4Property>(
                         translate("Color"), "desc", [this] { on_prop_change(); }, Float4Property::Type::color_rgba)}

          },
          graph) {}

auto UniformColorNode::execute() -> void {
  auto &processor = exe_ctx.value()->uniform_color_processor;
  processor.set_prop("color", props.get_prop<Float4Property>("color"));
  const auto &[width, height] = common_props.get_prop<Integer2Property>("output_size").value();
  processor.execute(outputs[0].cache_frame_buf.value(), outputs[0].cache_tex.value(), width, height);
}

using BlendMode = BlendNode::BlendMode;

const EnumItems BlendNode::BLENDING_MODES = {{"Add Sub", "", BlendMode::add_sub},
                                             {"Copy", "", BlendMode::copy},
                                             {"Multiply", "", BlendMode::multiply},
                                             {"Screen", "", BlendMode::screen},
                                             {"Overlay", "", BlendMode::overlay},
                                             {"Hard Light", "", BlendMode::hard_light},
                                             {"Soft Light", "", BlendMode::soft_light},
                                             {"Color Dodge", "", BlendMode::color_dodge},
                                             {"Linear Dodge", "", BlendMode::linear_dodge},
                                             {"Color Burn", "", BlendMode::color_burn},
                                             {"Linear Burn", "", BlendMode::linear_burn},
                                             {"Vivid Light", "", BlendMode::vivid_light},
                                             {"Divide", "", BlendMode::divide},
                                             {"Subtract", "", BlendMode::subtract},
                                             {"Difference", "", BlendMode::difference},
                                             {"Darken", "", BlendMode::darken},
                                             {"Lighten", "", BlendMode::lighten},
                                             {"Hue", "", BlendMode::hue},
                                             {"Saturation", "", BlendMode::saturation},
                                             {"Color", "", BlendMode::color},
                                             {"Luminosity", "", BlendMode::luminosity},
                                             {"Pin Light", "", BlendMode::pin_light},
                                             {"Hard Max", "", BlendMode::hard_max},
                                             {"Exclusion", "", BlendMode::exclusion}};

using AlphaMode = BlendNode::AlphaMode;

const EnumItems BlendNode::ALPHA_MODES = {{"Background", "", AlphaMode::background},
                                          {"Foreground", "", AlphaMode::foreground},
                                          {"Min", "", AlphaMode::min},
                                          {"Max", "", AlphaMode::max},
                                          {"Average", "", AlphaMode::average},
                                          {"Add", "", AlphaMode::add}};

BlendNode::BlendNode(UUID uuid, MaterialGraph *graph)
    : MaterialNode(uuid, translate("Blend"),
                   // Inputs
                   {MaterialInSocket(generate_uuid(), translate("Background"), MaterialSocketType::universal),
                    MaterialInSocket(generate_uuid(), translate("Foreground"), MaterialSocketType::universal),
                    MaterialInSocket(generate_uuid(), translate("Mask"), MaterialSocketType::grayscale)},
                   // Outputs
                   {MaterialOutSocket(generate_uuid(), translate("Output"), MaterialSocketType::color)},
                   // Props
                   {{"alpha", make_unique<FloatProperty>(
                                  translate("Alpha"), translate("Alpha of the blend"), [this] { execute(); },
                                  FloatProperty::Type::generic, 1.0F, 0.0F, 1.0F, 0.0F, 1.0F)},
                    {"blending_mode",
                     make_unique<EnumProperty>(translate("Blending Mode"), translate("Blending Mode"), BLENDING_MODES,
                                               BlendMode::add_sub, [this] { on_prop_change(); })},
                    {"alpha_mode", make_unique<EnumProperty>(translate("Alpha Mode"), translate("Alpha"), ALPHA_MODES,
                                                             AlphaMode::background, [this] { on_prop_change(); })}},
                   graph) {}

auto BlendNode::execute() -> void {
  AF_ASSERT(is_initialized)
  AF_ASSERT(exe_ctx.has_value())
  auto &proc = exe_ctx.value()->blend_processor;
  const auto &blend_mode = props.get_prop<EnumProperty>("blending_mode");
  proc.set_prop("blendMode", blend_mode);
  const auto &alpha = props.get_prop<FloatProperty>("alpha");
  proc.set_prop("alpha", alpha);
  const auto &alpha_mode = props.get_prop<EnumProperty>("alpha_mode");
  proc.set_prop("alphaMode", alpha_mode);
  const auto &mask_soc = inputs[2];
  glActiveTexture(GL_TEXTURE0);
  if (mask_soc.link_uuid.has_value()) {
    const auto texture = mask_soc.get_texture(graph);
    glBindTexture(GL_TEXTURE_2D, texture);
    proc.set_uniform("hasMask", 1);
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
    proc.set_uniform("hasMask", 0);
  }
  proc.set_texture("Mask", 0);
  const auto &bg_socket = inputs[0];
  const auto &fg_socket = inputs[1];
  glActiveTexture(GL_TEXTURE1);
  if (bg_socket.link_uuid.has_value()) {
    glBindTexture(GL_TEXTURE_2D, bg_socket.get_texture(graph));
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  proc.set_texture("Background", 1);
  glActiveTexture(GL_TEXTURE2);
  if (fg_socket.link_uuid.has_value()) {
    glBindTexture(GL_TEXTURE_2D, fg_socket.get_texture(graph));

  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  proc.set_texture("Foreground", 2);
  const auto [width, height] = common_props.get_prop<Integer2Property>("output_size").value();
  proc.execute(outputs[0].cache_frame_buf.value(), outputs[0].cache_tex.value(), width, height);
}

BlurNode::BlurNode(UUID uuid, MaterialGraph *graph)
    : MaterialNode(
          uuid, translate("Blur"),
          // Inputs
          {MaterialInSocket(generate_uuid(), translate("Input"), MaterialSocketType::color)},
          // Outputs
          {MaterialOutSocket(generate_uuid(), translate("Output"), MaterialSocketType::color)},
          // Props
          {{"pixel_shape", make_unique<Float2Property>(
                               translate("Box Shape"), translate("Shape of the box"), Float2Property::Type::generic,
                               [this] { execute(); }, FVec2{0.0F, 1.0F}, 0.0F, 1.0F, 0.0F, 1.0F)},
           {"radius", make_unique<FloatProperty>(
                          translate("Radius"), translate("Radius of the blur"), [this] { execute(); },
                          FloatProperty::Type::generic, 1.0F, 0.0F, 10.0F, 0.0F)}},
          graph) {}

auto BlurNode::execute() -> void {
  AF_ASSERT(is_initialized)
  AF_ASSERT(exe_ctx.has_value())
  auto &proc = exe_ctx.value()->blur_processor;
  const auto &radius = props.get_prop<FloatProperty>("radius");
  proc.set_prop("intensity", radius);
  const auto &pixel_shape = props.get_prop<Float2Property>("pixel_shape");
  proc.set_prop("pixel_shape", pixel_shape);
  const auto &fg_socket = inputs[0];
  glActiveTexture(GL_TEXTURE0);
  if (fg_socket.link_uuid.has_value()) {
    glBindTexture(GL_TEXTURE_2D, fg_socket.get_texture(graph));

  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  proc.set_texture("MainTex", 0);
  const auto [width, height] = common_props.get_prop<Integer2Property>("output_size").value();
  proc.execute(outputs[0].cache_frame_buf.value(), outputs[0].cache_tex.value(), width, height);
}

const EnumItems ChannelSelectNode::CHANNELS = {
    {"Input 1 red", "Use the red channel of input 1 as source", Channel::red_1},
    {"Input 1 green", "Use the green channel of input 1 as source", Channel::green_1},
    {"Input 1 blue", "Use the blue channel of input 1 as source", Channel::blue_1},
    {"Input 1 alpha", "Use the alpha channel of input 1 as source", Channel::alpha_1},
    {"Input 2 red", "Use the red channel of input 2 as source", Channel::red_2},
    {"Input 2 green", "Use the green channel of input 2 as source", Channel::green_2},
    {"Input 2 blue", "Use the blue channel of input 2 as source", Channel::blue_2},
    {"Input 2 alpha", "Use the alpha channel of input 2 as source", Channel::alpha_2}};

ChannelSelectNode::ChannelSelectNode(UUID uuid, MaterialGraph *graph)
    : MaterialNode(uuid, translate("Channel Select"),
                   // Inputs
                   {MaterialInSocket(generate_uuid(), translate("Input 1"), MaterialSocketType::universal),
                    MaterialInSocket(generate_uuid(), translate("Input 2"), MaterialSocketType::universal)},
                   // Outputs
                   {MaterialOutSocket(generate_uuid(), translate("Output"), MaterialSocketType::color)},
                   // Props
                   {{"channel_red", make_unique<EnumProperty>(translate("Red Channel"),
                                                              translate("Source for the red channel of the output"),
                                                              CHANNELS, Channel::red_1, [this] { execute(); })},
                    {"channel_green", make_unique<EnumProperty>(translate("Green Channel"),
                                                                translate("Source for the green channel of the output"),
                                                                CHANNELS, Channel::green_1, [this] { execute(); })},
                    {"channel_blue", make_unique<EnumProperty>(translate("Blue Channel"),
                                                               translate("Source for the blue channel of the output"),
                                                               CHANNELS, Channel::blue_1, [this] { execute(); })},
                    {"channel_alpha", make_unique<EnumProperty>(translate("Alpha Channel"),
                                                                translate("Source for the alpha channel of the output"),
                                                                CHANNELS, Channel::alpha_1, [this] { execute(); })}},
                   graph) {}

auto ChannelSelectNode::execute() -> void {
  AF_ASSERT(is_initialized)
  AF_ASSERT(exe_ctx.has_value())
  auto &proc = exe_ctx.value()->channel_select_processor;
  const auto &channel_red = props.get_prop<EnumProperty>("channel_red");
  const auto &channel_green = props.get_prop<EnumProperty>("channel_green");
  const auto &channel_blue = props.get_prop<EnumProperty>("channel_blue");
  const auto &channel_alpha = props.get_prop<EnumProperty>("channel_alpha");
  proc.set_prop("channel_red", channel_red);
  proc.set_prop("channel_green", channel_green);
  proc.set_prop("channel_blue", channel_blue);
  proc.set_prop("channel_alpha", channel_alpha);
  const auto &input1_socket = inputs[0];
  const auto &input2_socket = inputs[1];
  glActiveTexture(GL_TEXTURE0);
  if (input1_socket.link_uuid.has_value()) {
    glBindTexture(GL_TEXTURE_2D, input1_socket.get_texture(graph));

  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  proc.set_texture("input1", 0);
  glActiveTexture(GL_TEXTURE1);
  if (input2_socket.link_uuid.has_value()) {
    glBindTexture(GL_TEXTURE_2D, input2_socket.get_texture(graph));

  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  proc.set_texture("input2", 1);
  const auto [width, height] = common_props.get_prop<Integer2Property>("output_size").value();
  proc.execute(outputs[0].cache_frame_buf.value(), outputs[0].cache_tex.value(), width, height);
}

CurveNode::CurveNode(UUID uuid, MaterialGraph *graph)
    : MaterialNode(
          uuid, translate("Curve"),
          // Inputs
          {MaterialInSocket(generate_uuid(), translate("Input"), MaterialSocketType::universal)},
          // Outputs
          {MaterialOutSocket(generate_uuid(), translate("Output"), MaterialSocketType::color)},
          // Props
          {{"apply_curve", make_unique<BoolProperty>(
                               translate("Apply Curve"),
                               translate("Apply the curve to the input if turned on; otherwise, show the curve"),
                               [this] { execute(); }, false)},
           {"curve_addressing", make_unique<EnumProperty>(
                                    translate("Curve Addressing"),
                                    translate("Specifies how values out of the 0.0 to 1.0 range are handled"),
                                    EnumItems{{translate("Clamp"), translate("Clamp"), (int)CurveAddressing::clamp},
                                              {translate("Repeat"), translate("Repeat"), (int)CurveAddressing::repeat}},
                                    (int)CurveAddressing::clamp, [this] { execute(); })},
           {"curve", make_unique<CurveProperty>(translate("Curve"), translate("Curve to use"), [this] { execute(); })}},
          graph) {}

auto CurveNode::execute() -> void {
  AF_ASSERT(is_initialized)
  AF_ASSERT(exe_ctx.has_value())
  constexpr auto lut_size = 1024;
  const auto apply_curve = props.get_prop<BoolProperty>("apply_curve");
  auto &input_socket = inputs[0];
  // Early return on no input
  if (apply_curve.value() && !input_socket.is_linked()) {
    return;
  }
  auto &proc = exe_ctx.value()->curve_processor;
  const auto &curve = props.get_prop<CurveProperty>("curve");
  const auto lum_lut = curve.value(CurveProperty::Channel::lum).lut(lut_size);
  const auto red_lut = curve.value(CurveProperty::Channel::r).lut(lut_size);
  const auto green_lut = curve.value(CurveProperty::Channel::g).lut(lut_size);
  const auto blue_lut = curve.value(CurveProperty::Channel::b).lut(lut_size);
  const auto alpha_lut = curve.value(CurveProperty::Channel::a).lut(lut_size);
  auto main_lut = std::vector<float>(static_cast<size_t>(lut_size) * 4);

  for (auto i = 0, x = 0; i < (lut_size * 4); i += 4, ++x) {
    main_lut[i] = red_lut[x];
    main_lut[i + 1] = green_lut[x];
    main_lut[i + 2] = blue_lut[x];
    main_lut[i + 3] = alpha_lut[x];
  }
  GLuint lut_tex = 0;
  glGenTextures(1, &lut_tex);
  glBindTexture(GL_TEXTURE_1D, lut_tex);
  GLenum texture_wrap = GL_CLAMP_TO_EDGE;
  const auto addressing = (CurveAddressing)props.get_prop<EnumProperty>("curve_addressing").value();
  if (addressing == CurveAddressing::clamp) {
    texture_wrap = GL_CLAMP_TO_EDGE;
  } else {
    texture_wrap = GL_REPEAT;
  }
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, texture_wrap);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, texture_wrap);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, lut_size, 0, GL_RED, GL_FLOAT, lum_lut.data());

  GLuint main_lut_tex = 0;
  glGenTextures(1, &main_lut_tex);
  glBindTexture(GL_TEXTURE_1D, main_lut_tex);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, texture_wrap);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, texture_wrap);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, lut_size, 0, GL_RGBA, GL_FLOAT, main_lut.data());
  proc.set_prop("apply_curve", apply_curve);
  glActiveTexture(GL_TEXTURE0);
  if (input_socket.link_uuid.has_value()) {
    glBindTexture(GL_TEXTURE_2D, input_socket.get_texture(graph));

  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  proc.set_texture("input_tex", 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, lut_tex);
  proc.set_texture("lum_curve_lut", 1);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, main_lut_tex);
  proc.set_texture("main_curve_lut", 2);

  const auto [width, height] = common_props.get_prop<Integer2Property>("output_size").value();
  proc.execute(outputs[0].cache_frame_buf.value(), outputs[0].cache_tex.value(), width, height);
  glDeleteTextures(1, &lut_tex);
  glDeleteTextures(1, &main_lut_tex);
}

}  // namespace afro::core::material_nodes