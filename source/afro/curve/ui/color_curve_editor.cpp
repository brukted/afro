/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "color_curve_editor.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>

#include <array>

#include "curve/data/curve.h"
#include "utils/assert.h"
#include "utils/math.h"
#include "utils/translation.h"

using namespace ImGui;

namespace afro::curve {

auto draw_curve_selector(ColorCurve& color_curve,
                         std::array<ImColor, 5> curve_colors)
    -> std::pair<BezierSpline*, ImU32> {
  ImGuiStorage* storage = GetStateStorage();
  const auto current_curve_strg_id = GetID("current_curve");
  const auto curve_color_strg_id = GetID("curve_color");
  auto* curve = (BezierSpline*)storage->GetVoidPtr(current_curve_strg_id);
  const auto curve_color =
      (ImU32)curve_colors.at(storage->GetInt(curve_color_strg_id));

  if (curve == nullptr) {
    curve = &color_curve.lum;
    storage->SetVoidPtr(current_curve_strg_id, curve);
  }
  static std::array<const char*, 5> curve_names = {
      translate("Luminance"), translate("Red"), translate("Green"),
      translate("Blue"), translate("Alpha")};

  BeginGroup();
  if (BeginCombo("###curve_selector",
                 curve_names.at(storage->GetInt(curve_color_strg_id)),
                 ImGuiComboFlags_NoArrowButton)) {
    if (Selectable(translate("Luminance"), curve == &color_curve.lum)) {
      curve = &color_curve.lum;
      storage->SetInt(curve_color_strg_id, 0);
    }
    if (Selectable(translate("Red"), curve == &color_curve.r)) {
      curve = &color_curve.r;
      storage->SetInt(curve_color_strg_id, 1);
    }
    if (Selectable(translate("Green"), curve == &color_curve.g)) {
      curve = &color_curve.g;
      storage->SetInt(curve_color_strg_id, 2);
    }
    if (Selectable(translate("Blue"), curve == &color_curve.b)) {
      curve = &color_curve.b;
      storage->SetInt(curve_color_strg_id, 3);
    }
    if (Selectable(translate("Alpha"), curve == &color_curve.a)) {
      curve = &color_curve.a;
      storage->SetInt(curve_color_strg_id, 4);
    }
    storage->SetVoidPtr(current_curve_strg_id, curve);
    EndCombo();
  }
  EndGroup();

  return {curve, curve_color};
}

auto draw_color_curve_editor(ColorCurve& color_curve) -> bool {
  // TODO (afro): Make user-configurable.
  const auto gradient_width = 20.0F;
  const auto gradient_padding = 5.0F;
  const auto control_point_treshold = 50.0F;
  const auto grid_spacing = 0.1F;
  const auto grid_line_width = 1.0F;
  const auto grid_line_color = (ImU32)ImColor(70, 70, 70);
  const auto editor_bg_color = (ImU32)ImColor(0, 0, 0);
  const auto handle_radius = 5.0F;
  const auto handle_color = (ImU32)ImColor(255, 255, 255);
  const auto tangent_line_color = ImU32(0xFFFFFFFF);
  static const std::array<ImColor, 5> curve_colors = {
      ImColor(128, 128, 128), ImColor(255, 0, 0), ImColor(0, 255, 0),
      ImColor(0, 0, 255), ImColor(255, 255, 255)};
  const auto curve_thickness = 2.0F;
  bool has_any_curve_changed = false;

  // Start the color curve editor.
  PushID(&color_curve);

  auto const [cr, col] = draw_curve_selector(color_curve, curve_colors);
  BezierSpline* curve = cr;
  const ImU32 curve_color = col;

  auto frame_size = GetCurrentWindow()->WorkRect.GetSize();
  frame_size = {frame_size.x, frame_size.x};

  if (!BeginChildFrame(GetCurrentWindow()->GetID("editor_child_frame"),
                       frame_size)) {
    EndChildFrame();
    return false;
  }
  frame_size = GetCurrentWindow()->WorkRect.GetSize();

  auto* draw_list = GetWindowDrawList();
  const auto frame_upper_left = GetCursorScreenPos();
  const ImVec2 editor_size = {frame_size.x - gradient_padding - gradient_width,
                              frame_size.x - gradient_padding - gradient_width};
  const ImVec2 editor_lower_left = {
      frame_upper_left.x + gradient_width + gradient_padding,
      frame_upper_left.y + editor_size.y};

  // Draws gradients bars
  {
    static const auto black = ImColor(255, 255, 255);
    static const auto white = ImColor(0, 0, 0);
    draw_list->AddRectFilledMultiColor(frame_upper_left,
                                       {frame_upper_left.x + gradient_width,
                                        frame_upper_left.y + editor_size.y},
                                       black, black, white, white);
    draw_list->AddRectFilledMultiColor(
        {frame_upper_left.x + gradient_width + gradient_padding,
         frame_upper_left.y + editor_size.y + gradient_padding},
        {frame_upper_left.x + gradient_width + gradient_padding + editor_size.x,
         frame_upper_left.y + editor_size.y + gradient_padding +
             gradient_width},
        white, black, black, white);
  }

  // Draws Editor background & grid
  {
    const ImVec2 editor_upper_left = {
        frame_upper_left.x + gradient_width + gradient_padding,
        frame_upper_left.y};
    ImVec2 editor_lower_right = {frame_upper_left.x + frame_size.x,
                                 frame_upper_left.y + editor_size.y};
    draw_list->AddRectFilled(editor_upper_left, editor_lower_right,
                             (ImU32)editor_bg_color);

    auto upper_corner = GetCursorScreenPos();
    upper_corner.x += gradient_width + gradient_padding;

    for (float dx = 0; dx < 1.0F; dx += grid_spacing) {
      draw_list->AddLine(
          {dx * editor_size.x + upper_corner.x, upper_corner.y},
          {dx * editor_size.x + upper_corner.x, upper_corner.y + editor_size.y},
          grid_line_color, grid_line_width);
    }
    for (float dy = 0; dy < 1.0F; dy += grid_spacing) {
      draw_list->AddLine(
          {upper_corner.x, upper_corner.y + dy * editor_size.y},
          {upper_corner.x + editor_size.x, upper_corner.y + dy * editor_size.y},
          grid_line_color, grid_line_width);
    }
    draw_list->AddRect(editor_upper_left, editor_lower_right, grid_line_color,
                       0.F, 0, grid_line_width);
  }

  // Draws control points
  {
    // Transforms control points to screen space
    auto transform = [editor_lower_left,
                      editor_size](const FVec2& p) -> ImVec2 {
      auto ip = ImVec2();
      ip.x = editor_lower_left.x + (p.x * editor_size.x);
      ip.y = editor_lower_left.y - (p.y * editor_size.y);
      return ip;
    };

    // Transforms @a p from screen space to curve space
    auto inverse_transform = [&](const ImVec2& p) -> FVec2 {
      auto ip = FVec2();
      ip.x = (p.x - editor_lower_left.x) / editor_size.x;
      ip.y = (editor_lower_left.y - p.y) / editor_size.y;
      return ip;
    };

    auto draggable_point = [&](const char* str_id, FVec2& point,
                               ImVec2& screen_point,
                               RangeF x_range = {0.0F, 1.0F},
                               RangeF y_range = {0.0F, 1.0F}) -> bool {
      const auto mouse_pos = GetIO().MousePos;
      const auto delta =
          ImVec2(mouse_pos.x - screen_point.x, mouse_pos.y - screen_point.y);
      const auto distance = ImSqrt(ImLengthSqr(delta));
      if (distance > control_point_treshold) {
        return false;
      }
      SetCursorScreenPos(screen_point - ImVec2{handle_radius, handle_radius});
      InvisibleButton(str_id, {handle_radius * 4, handle_radius * 4});
      if (IsItemActive() && IsWindowFocused() &&
          IsMouseDown(ImGuiMouseButton_Left)) {
        screen_point = mouse_pos;
        point = inverse_transform(screen_point);
        x_range.clamp(point.x);
        y_range.clamp(point.y);
        return true;
      }
      return false;
    };

    auto draw_curve = [&](BezierSpline& m_curve) -> bool {
      bool has_changed = false;
      for (int i = 1; i < m_curve.points.size() - 1; ++i) {
        PushID(i);
        auto& point = m_curve.points[i];
        ImVec2 pos = transform(point.pos);
        if (draggable_point("pos", point.pos, pos)) {
          has_changed = true;
          const auto mouse_delta = GetIO().MouseDelta;
          const FVec2 pos_delta = {mouse_delta.x / editor_size.x,
                                   -mouse_delta.y / editor_size.y};
          point.t1 = point.t1 + pos_delta;
          point.t2 = point.t2 + pos_delta;
        }
        ImVec2 t1 = transform(point.t1);
        has_changed |= draggable_point("t1", point.t1, t1, {0.0F, point.pos.x});
        ImVec2 t2 = transform(point.t2);
        has_changed |= draggable_point("t2", point.t2, t2, {point.pos.x, 1.0F});
        draw_list->AddCircleFilled(pos, handle_radius, handle_color);
        draw_list->AddCircleFilled(t1, handle_radius, handle_color);
        draw_list->AddCircleFilled(t2, handle_radius, handle_color);
        draw_list->AddLine(t1, pos, tangent_line_color);
        draw_list->AddLine(t2, pos, tangent_line_color);
        PopID();
      }

      for (size_t i = 1; i < m_curve.points.size(); ++i) {
        const auto& point = m_curve.points[i];
        const auto& previous_point = m_curve.points[i - 1];
        draw_list->AddBezierCubic(transform(previous_point.pos),
                                  transform(previous_point.t2),
                                  transform(point.t1), transform(point.pos),
                                  curve_color, curve_thickness);
      }

      // Split curve interactivity
      if (IsWindowHovered() && IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        const auto mouse_pos = inverse_transform(GetMousePos());
        if (mouse_pos.x >= 0.0F && mouse_pos.x <= 1.0F) {
          m_curve.split_at(mouse_pos.x);
          has_changed = true;
        }
      }

      // Debug
      if (IsWindowHovered() && IsMouseDown(ImGuiMouseButton_Right)) {
        const auto mouse_pos = inverse_transform(GetMousePos());
        if (mouse_pos.x >= 0.0F && mouse_pos.x <= 1.0F) {
          draw_list->AddCircleFilled(
              transform({mouse_pos.x, m_curve.at(mouse_pos.x)}),
              curve_thickness + 3.0F, curve_color);
        }
      }

      if (has_changed) {
        m_curve.sort();
        m_curve.validate();
      }

      return has_changed;
    };
    has_any_curve_changed |= draw_curve(*curve);
  }
  EndChildFrame();
  PopID();
  return has_any_curve_changed;
}
}  // namespace afro::curve
