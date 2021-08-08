/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include <array>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#ifdef _WIN32
#include <shlobj_core.h>  // SHGetKnownFolderPath
#include <windows.h>      //GetModuleFileNameW
#endif                    // _WIN32

#ifdef __linux__
#include <limits.h>
#include <unistd.h>  //readlink
#endif               //__linux__

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif  // __APPLE__

#include "paths.h"
#include "utils/build_info.h"

namespace fs = std::filesystem;

namespace afro::paths {

auto log_dir() -> fs::path { return temp_dir() / "afro" / "log"; }

auto temp_dir() -> fs::path { return (fs::temp_directory_path() / "afro"); }

auto data_dir() -> fs::path {
#if defined(_WIN32)
  auto path = fs::absolute(exe_path().parent_path()) / "data";
#elif defied(__linux__)
#error "data_dir() not implemented on this platform"
#elif defined(__APPLE__)
#error "data_dir() not implemented on this platform"
#else
#error "data_dir() not implemented on this platform"
#endif
  return path;
}

auto sys_addons_dir() -> fs::path {
#if defined(_WIN32)
  auto path = data_dir() / "addons";
#elif defied(__linux__)
#error "sys_addon_dir() not implemented on this platform"
#elif defined(__APPLE__)
#error "sys_addon_dir() not implemented on this platform"
#else
#error "sys_addon_dir() not implemented on this platform"
#endif
  return path;
}

auto user_addons_dir() -> fs::path { return (user_data_path() / "addons"); }

auto config_dir() -> fs::path { return (user_data_path() / "config"); }

auto user_data_path() -> fs::path {
  fs::path path{""};
#if defined(_WIN32)
  PWSTR buffer = nullptr;
  auto result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &buffer);
  if (SUCCEEDED(result)) {
    auto wstr = std::wstring_view(buffer);
    path = fs::path(wstr.begin(), wstr.end()) / "afro";
    CoTaskMemFree(buffer);
  }
#elif defined(__linux__)
#error "user_data_path() not implemented on this platform"
#elif defined(__APPLE__)
#error "user_data_path() not implemented on this platform"
#else
#error "user_data_path() not implemented on this platform"
#endif

  return path;
}

auto exe_path() -> fs::path {
#if defined(_WIN32)
  std::array<char, MAX_PATH> buffer = {""};

  // When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
  HMODULE h_module = GetModuleHandle(nullptr);
  if (h_module != nullptr) {
    // Use GetModuleFileName() with module handle to get the path
    GetModuleFileName(h_module, buffer.data(), (sizeof(buffer)));
    return buffer.data();
  }

#elif defined(__linux__)
  std::array<char, PATH_MAX> buffer = {""};
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
  if (count != -1) return buffer.data();

#elif defined(__APPLE__)
  std::vector<char> buffer;
  unsigned int buffer_size;
  path.resize(buffer_size);
  if (_NSGetExecutablePath(&buffer[0], &bufferSize)) {
    buffer.resize(bufferSize);
    _NSGetExecutablePath(&buffer[0], &bufferSize);
  }
  return path.data();
#else
#error "exe_path() not implemented on this platform"
#endif
  return "";
}

auto cache_dir() -> fs::path { return ""; }

}  // namespace afro::paths
