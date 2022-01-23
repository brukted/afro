/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "preferences.h"

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/details/helpers.hpp>
#include <filesystem>
#include <fstream>
#include <utility>

#include "utils/log.h"

namespace fs = std::filesystem;

namespace afro {
auto Preferences::load_from_file(std::string pref_path) -> void {
  file_path = std::move(pref_path);
  std::ifstream ifs(file_path);
  if (!ifs.is_open()) {
    log::core_error("Failed to open preferences file: {}", file_path);
    log::core_error("Failed to load preferences");
    return;
  }
  cereal::JSONInputArchive archive(ifs);
  try {
    archive(cereal::make_nvp("preferences", *this));
  } catch (std::exception& e) {
    log::core_error("Failed to load preferences: {}", e.what());
  }
}

auto Preferences::save_to_file() -> void {
  std::ofstream file(file_path);
  if (!file.is_open()) {
    log::core_error("Failed to open preferences file: {}", file_path);
    log::core_error("Failed to save preferences");
    return;
  }
  cereal::JSONOutputArchive archive(file);
  archive(cereal::make_nvp("preferences", *this));
}

auto Preferences::get() -> Preferences& {
  static Preferences prefs;
  return prefs;
}

}  // namespace afro

namespace cereal {
template <class Archive>
void serialize(Archive& archive, afro::Preferences& prefs) {
  archive(make_nvp("theme", prefs.theme));
}
template <class Archive>
void serialize(Archive& archive, ImGuiStyle& style) {
  archive(make_nvp("Alpha", style.Alpha), make_nvp("Disabled Alpha", style.DisabledAlpha),
          make_nvp("Window Padding", style.WindowPadding), make_nvp("Window Rounding", style.WindowRounding),
          make_nvp("Window Border Size", style.WindowBorderSize), make_nvp("Window Minimum Size", style.WindowMinSize),
          // TODO WindowMenuButtonPosition
          make_nvp("Window Title Align", style.WindowTitleAlign), make_nvp("Child Rounding", style.ChildRounding),
          make_nvp("Child Border Size", style.ChildBorderSize), make_nvp("Popup Rounding", style.PopupRounding),
          make_nvp("Popup Border Size", style.PopupBorderSize), make_nvp("Frame Padding", style.FramePadding),
          make_nvp("Frame Rounding", style.FrameRounding), make_nvp("Frame Border Size", style.FrameBorderSize),
          make_nvp("Item Spacing", style.ItemSpacing), make_nvp("Item Inner Spacing", style.ItemInnerSpacing),
          make_nvp("Cell Padding", style.CellPadding), make_nvp("Touch Extra Padding", style.TouchExtraPadding),
          make_nvp("Indent Spacing", style.IndentSpacing), make_nvp("Columns Min Spacing", style.ColumnsMinSpacing),
          make_nvp("Scrollbar Size", style.ScrollbarSize), make_nvp("Scrollbar Rounding", style.ScrollbarRounding),
          make_nvp("Grab Min Size", style.GrabMinSize), make_nvp("Grab Rounding", style.GrabRounding),
          make_nvp("Log Slider Deadzone", style.LogSliderDeadzone), make_nvp("Tab Rounding", style.TabRounding),
          make_nvp("Tab Border Size", style.TabBorderSize),
          make_nvp("Tab Min Width For Close Button", style.TabMinWidthForCloseButton),
          // TODO ColorButtonPosition
          make_nvp("Button Text Align", style.ButtonTextAlign),
          make_nvp("Selectable Text Align", style.SelectableTextAlign),
          make_nvp("Display Window Padding", style.DisplayWindowPadding),
          make_nvp("Display Safe Area Padding", style.DisplaySafeAreaPadding),
          make_nvp("Mouse Cursor Scale", style.MouseCursorScale),
          make_nvp("Anti-aliased Lines", style.AntiAliasedLines), make_nvp("Anti-aliased Fill", style.AntiAliasedFill),
          make_nvp("Curve Tessellation Tolerance", style.CurveTessellationTol),
          make_nvp("Circle Segment Max Error", style.CircleTessellationMaxError));

  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    archive(make_nvp(ImGui::GetStyleColorName(i), style.Colors[i]));
  }
}

template <class Archive>
void serialize(Archive& archive, ImVec2& vec) {
  archive(make_nvp("x", vec.x), make_nvp("y", vec.y));
}

template <class Archive>
void serialize(Archive& archive, ImVec4& vec) {
  archive(make_nvp("x", vec.x), make_nvp("y", vec.y), make_nvp("z", vec.z), make_nvp("w", vec.w));
}

template <class Archive>
void serialize(Archive& archive, afro::ui::Theme& theme) {
  archive(cereal::make_nvp("imgui_style", theme.imgui_style));
  archive(cereal::make_nvp("user_interface.tooltip.delay", theme.user_interface.tooltip.delay));
}
}  // namespace cereal