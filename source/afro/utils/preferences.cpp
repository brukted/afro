/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "preferences.h"

#include <cereal/archives/json.hpp>
#include <filesystem>
#include <fstream>
#include <utility>

#include "cereal/cereal.hpp"
#include "cereal/details/helpers.hpp"
#include "imgui.h"
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
  archive(cereal::make_nvp("Theme", prefs.theme));
}

template <class Archive>
void serialize(Archive& archive, ImGuiStyle& style) {
  archive(
      cereal::make_nvp("Alpha", style.Alpha), cereal::make_nvp("Disabled Alpha", style.DisabledAlpha),
      cereal::make_nvp("Window Padding", style.WindowPadding),
      cereal::make_nvp("Window Rounding", style.WindowRounding),
      cereal::make_nvp("Window Border Size", style.WindowBorderSize),
      cereal::make_nvp("Window Minimum Size", style.WindowMinSize),
      // TODO WindowMenuButtonPosition
      cereal::make_nvp("Window Title Align", style.WindowTitleAlign),
      cereal::make_nvp("Child Rounding", style.ChildRounding),
      cereal::make_nvp("Child Border Size", style.ChildBorderSize),
      cereal::make_nvp("Popup Rounding", style.PopupRounding),
      cereal::make_nvp("Popup Border Size", style.PopupBorderSize),
      cereal::make_nvp("Frame Padding", style.FramePadding), cereal::make_nvp("Frame Rounding", style.FrameRounding),
      cereal::make_nvp("Frame Border Size", style.FrameBorderSize), cereal::make_nvp("Item Spacing", style.ItemSpacing),
      cereal::make_nvp("Item Inner Spacing", style.ItemInnerSpacing),
      cereal::make_nvp("Cell Padding", style.CellPadding),
      cereal::make_nvp("Touch Extra Padding", style.TouchExtraPadding),
      cereal::make_nvp("Indent Spacing", style.IndentSpacing),
      cereal::make_nvp("Columns Min Spacing", style.ColumnsMinSpacing),
      cereal::make_nvp("Scrollbar Size", style.ScrollbarSize),
      cereal::make_nvp("Scrollbar Rounding", style.ScrollbarRounding),
      cereal::make_nvp("Grab Min Size", style.GrabMinSize), cereal::make_nvp("Grab Rounding", style.GrabRounding),
      cereal::make_nvp("Log Slider Deadzone", style.LogSliderDeadzone),
      cereal::make_nvp("Tab Rounding", style.TabRounding), cereal::make_nvp("Tab Border Size", style.TabBorderSize),
      cereal::make_nvp("Tab Min Width For Close Button", style.TabMinWidthForCloseButton),
      // TODO ColorButtonPosition
      cereal::make_nvp("Button Text Align", style.ButtonTextAlign),
      cereal::make_nvp("Selectable Text Align", style.SelectableTextAlign),
      cereal::make_nvp("Display Window Padding", style.DisplayWindowPadding),
      cereal::make_nvp("Display Safe Area Padding", style.DisplaySafeAreaPadding),
      cereal::make_nvp("Mouse Cursor Scale", style.MouseCursorScale),
      cereal::make_nvp("Anti-aliased Lines", style.AntiAliasedLines),
      cereal::make_nvp("Anti-aliased Fill", style.AntiAliasedFill),
      cereal::make_nvp("Curve Tessellation Tolerance", style.CurveTessellationTol),
      cereal::make_nvp("Circle Segment Max Error", style.CircleTessellationMaxError));

  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    archive(cereal::make_nvp(ImGui::GetStyleColorName(i), style.Colors[i]));
  }
}

template <class Archive>
void serialize(Archive& archive, ImVec2& vec) {
  archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y));
}
template <class Archive>
void serialize(Archive& archive, ImVec4& vec) {
  archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z),
          cereal::make_nvp("w", vec.w));
}
}  // namespace cereal