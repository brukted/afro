/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <filesystem>

namespace afro::paths {

auto log_dir() -> std::filesystem::path;
auto temp_dir() -> std::filesystem::path;
auto data_dir() -> std::filesystem::path;
auto config_dir() -> std::filesystem::path;
auto sys_addons_dir() -> std::filesystem::path;
auto user_addons_dir() -> std::filesystem::path;
auto user_data_path() -> std::filesystem::path;
auto exe_path() -> std::filesystem::path;
auto cache_dir() -> std::filesystem::path;

}  // namespace afro::paths
