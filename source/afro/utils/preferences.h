/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <imgui.h>

#include <any>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "utils/log.h"

namespace afro {
struct Preferences {
  std::string file_path;
  std::unordered_map<std::string, std::any> p_map;
  ImGuiStyle theme;
  auto load_from_file(std::string pref_path) -> void;
  auto save_to_file() -> void;

  template <typename T>
  auto put_preference(const std::string &path, T value) {
    p_map[path] = value;
  }

  template <typename T>
  auto get_preference(const std::string &path, T defaultValue) -> T {
    auto val = p_map.find(path);
    if (val == p_map.end()) {
      return defaultValue;
    }
    return std::any_cast<T>(val->second);
  }

  static auto get() -> Preferences &;
};
}  // namespace afro