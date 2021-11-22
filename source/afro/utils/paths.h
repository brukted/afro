/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <filesystem>

namespace afro::paths {

/**
 * @brief Path of the directory where log files are stored.
 *
 * @return std::filesystem::path
 */
auto log_dir() -> std::filesystem::path;
auto temp_dir() -> std::filesystem::path;
/**
 * @brief Path of the directory where afro's assets are stored.
 *
 * @return std::filesystem::path
 */
auto data_dir() -> std::filesystem::path;
/**
 * @brief Path of the directory where user's configuration is stored.
 *
 * @return std::filesystem::path
 */
auto config_dir() -> std::filesystem::path;
/**
 * @brief Path of the directory where afro's default addon files are stored.
 *
 * @return std::filesystem::path
 */
auto sys_addons_dir() -> std::filesystem::path;
/**
 * @brief Path of directory containing the user's addons.
 *
 */
auto user_addons_dir() -> std::filesystem::path;
/**
 * @brief Path of the root directory where the user's configuration is stored. On windows, this is
 * %APP_DATA%\afro.
 *
 * @return std::filesystem::path
 */
auto user_data_path() -> std::filesystem::path;
/**
 * @brief Path of the executable.
 *
 */
auto exe_path() -> std::filesystem::path;
/**
 * @brief Path of the cache directory
 *
 * @return std::filesystem::path
 */
auto cache_dir() -> std::filesystem::path;
inline auto sys_theme_dir() -> std::filesystem::path {
  auto path = data_dir() / "themes";
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  return path;
}

inline auto user_theme_dir() -> std::filesystem::path {
  auto path = user_data_path() / "themes";
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  return path;
}
}  // namespace afro::paths
