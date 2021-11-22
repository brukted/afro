/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "utils/preferences.h"
#include <imgui.h>
#include <string>

namespace afro::ui {
struct PreferencesEditor {
  enum Section : int { kTheme = 0, kFonts };
  struct ThemeFile {
    std::string name;
    std::string path;
    ImGuiStyle style;
  };

  Section section = kTheme;
  std::vector<ThemeFile> installed_themes;

  Preferences &preferences = Preferences::get();

  PreferencesEditor();

  auto draw(bool *p_open) -> void;
  auto draw_theme() -> void;
  auto draw_fonts() -> void;
};
} // namespace afro::ui