/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "preferences_editor.h"

#include <imgui.h>

#include <filesystem>

#include "utils/paths.h"
#include "utils/preferences.h"
#include "utils/translation.h"

namespace fs = std::filesystem;

namespace afro::ui {
PreferencesEditor::PreferencesEditor() {
  for (auto file_path : fs::recursive_directory_iterator(paths::sys_theme_dir())) {
  }
}

auto PreferencesEditor::draw(bool *p_open) -> void {
  if (!ImGui::Begin("Preferences", p_open)) {
    ImGui::End();
    return;
  }

  // Save load
  if (ImGui::Button(translate("Save"))) {
    Preferences::get().save_to_file();
  }
  ImGui::SameLine();
  if (ImGui::Button(translate("Reset"))) {
    Preferences::get().load_from_file(Preferences::get().file_path);
  }

  // Sections
  {
    ImGui::BeginChild("sections", {150, 0}, true);
    if (ImGui::Selectable(translate("Theme"), section == kTheme)) {
      section = kTheme;
    }
    if (ImGui::Selectable(translate("Fonts"), section == kFonts)) {
      section = kFonts;
    }
    ImGui::EndChild();
  }
  ImGui::SameLine();
  {
    switch (section) {
      case kTheme:
        draw_theme();
        break;
      case kFonts:
        draw_fonts();
        break;
    }
  }

  ImGui::End();
}

auto PreferencesEditor::draw_theme() -> void {
  if (!ImGui::BeginChild("Theme", {0, 0}, true)) {
    ImGui::EndChild();
    return;
  }
  ImGui::TextUnformatted(translate("Theme"));
  ImGui::TextUnformatted(translate("Themes"));
  ImGui::SameLine();
  if (ImGui::BeginCombo("###themes_combo", nullptr, ImGuiComboFlags_NoPreview)) {
    ImGui::EndCombo();
  }
  ImGuiStyle *ref = &Preferences::get().theme;

  // You can pass in a reference ImGuiStyle structure to compare to, revert to
  // and save to (else it compares to an internally stored reference)
  ImGuiStyle &style = ImGui::GetStyle();
  ImGuiStyle &ref_saved_style = Preferences::get().theme;

  // Default to using internal storage as reference
  static bool init = true;
  if (init /* && ref == nullptr*/) {
    ref_saved_style = style;
  }
  init = false;
  // if (ref == nullptr)
  ref = &ref_saved_style;

  ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50F);

  ImGui::SameLine();

  ImGui::Separator();

  if (ImGui::CollapsingHeader(translate("Sizes", "preferences_editor"))) {
    ImGui::TextUnformatted(translate("Main", "preferences_editor"));
    ImGui::SliderFloat2("WindowPadding", (float *)&style.WindowPadding, 0.0F, 20.0F, "%.0f");
    ImGui::SliderFloat2("FramePadding", (float *)&style.FramePadding, 0.0F, 20.0F, "%.0f");
    ImGui::SliderFloat2("ItemSpacing", (float *)&style.ItemSpacing, 0.0F, 20.0F, "%.0f");
    ImGui::SliderFloat2("ItemInnerSpacing", (float *)&style.ItemInnerSpacing, 0.0F, 20.0F, "%.0f");
    ImGui::SliderFloat2("TouchExtraPadding", (float *)&style.TouchExtraPadding, 0.0F, 10.0F, "%.0f");
    ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0F, 30.0F, "%.0f");
    ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0F, 20.0F, "%.0f");
    ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0F, 20.0F, "%.0f");
    ImGui::TextUnformatted(translate("Borders", "preferences_editor"));
    ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0F, 1.0F, "%.0f");
    ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0F, 1.0F, "%.0f");
    ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0F, 1.0F, "%.0f");
    ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0F, 1.0F, "%.0f");
    ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0F, 1.0F, "%.0f");
    ImGui::TextUnformatted(translate("Rounding", "prefernces_editor"));
    ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0F, 12.0F, "%.0f");
    ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0F, 12.0F, "%.0f");
    ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0F, 12.0F, "%.0f");
    ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0F, 12.0F, "%.0f");
    ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0F, 12.0F, "%.0f");
    ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0F, 12.0F, "%.0f");
    ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0F, 12.0F, "%.0f");
    ImGui::TextUnformatted(translate("Alignment", "preferences_editor"));
    ImGui::SliderFloat2("WindowTitleAlign", (float *)&style.WindowTitleAlign, 0.0F, 1.0F, "%.2f");
    int window_menu_button_position = style.WindowMenuButtonPosition + 1;
    if (ImGui::Combo("WindowMenuButtonPosition", (int *)&window_menu_button_position, "None\0Left\0Right\0")) {
      style.WindowMenuButtonPosition = window_menu_button_position - 1;
    }
    ImGui::Combo("ColorButtonPosition", (int *)&style.ColorButtonPosition, "Left\0Right\0");
    ImGui::SliderFloat2("ButtonTextAlign", (float *)&style.ButtonTextAlign, 0.0F, 1.0F, "%.2f");
    ImGui::SliderFloat2("SelectableTextAlign", (float *)&style.SelectableTextAlign, 0.0F, 1.0F, "%.2f");
    ImGui::SliderFloat2("DisplaySafeAreaPadding", (float *)&style.DisplaySafeAreaPadding, 0.0F, 30.0F, "%.0f");
  }

  if (ImGui::CollapsingHeader(translate("Colors", "preferences_editor"))) {
    static ImGuiTextFilter filter;
    filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

    static ImGuiColorEditFlags alpha_flags = 0;
    if (ImGui::RadioButton("Opaque", alpha_flags == 0)) {
      alpha_flags = 0;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) {
      alpha_flags = ImGuiColorEditFlags_AlphaPreview;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) {
      alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf;
    }

    ImGui::BeginChild("##colors", ImVec2(0, 0), true);
    ImGui::PushItemWidth(400);
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
      const char *name = ImGui::GetStyleColorName(i);
      if (!filter.PassFilter(name)) {
        continue;
      }
      ImGui::PushID(i);

      ImGui::ColorEdit4("##color", (float *)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
      if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0) {
        ImGui::SameLine(0.0F, style.ItemInnerSpacing.x);
        if (ImGui::Button("Save")) {
          ref->Colors[i] = style.Colors[i];
        }
        ImGui::SameLine(0.0F, style.ItemInnerSpacing.x);
        if (ImGui::Button("Revert")) {
          style.Colors[i] = ref->Colors[i];
        }
      }
      ImGui::SameLine(0.0F, style.ItemInnerSpacing.x);
      ImGui::TextUnformatted(name);
      ImGui::PopID();
    }
    ImGui::PopItemWidth();
    ImGui::EndChild();
  }
  ImGui::PopItemWidth();
  ImGui::EndChild();
}

auto PreferencesEditor::draw_fonts() -> void {}
}  // namespace afro::ui