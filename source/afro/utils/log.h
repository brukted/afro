/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "spdlog/logger.h"

namespace afro::log {

template <typename... Args>
using format_string_t = fmt::format_string<Args...>;

enum class LogLevel {
  trace = spdlog::level::trace,
  warn = spdlog::level::warn,
  info = spdlog::level::info,
  debug = spdlog::level::debug,
  error = spdlog::level::err,
  critical = spdlog::level::critical
};

struct Logger {
  std::shared_ptr<spdlog::logger> addon_logger;
  std::shared_ptr<spdlog::logger> core_logger;
  std::shared_ptr<spdlog::logger> gl_logger;
};

auto get_logger() -> Logger &;

auto init_log(Logger &, LogLevel) -> void;

auto set_log_level(LogLevel) -> void;

template <typename... Args>
auto addon_trace(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().addon_logger->trace(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto addon_warn(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().addon_logger->warn(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto addon_info(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().addon_logger->info(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto addon_debug(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().addon_logger->debug(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto addon_error(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().addon_logger->error(fmt, std::forward<Args>(args)...);
};

template <typename... Args>
auto addon_critical(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().addon_logger->critical(fmt,
                                                 std::forward<Args>(args)...);
};

template <typename... Args>
auto core_trace(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().core_logger->trace(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto core_warn(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().core_logger->warn(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto core_info(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().core_logger->info(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto core_debug(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().core_logger->debug(fmt, std::forward<Args>(args)...);
};
template <typename... Args>
auto core_error(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().core_logger->error(fmt, std::forward<Args>(args)...);
};

template <typename... Args>
auto core_critical(format_string_t<Args...> fmt, Args &&...args) -> void {
  afro::log::get_logger().core_logger->critical(fmt,
                                                std::forward<Args>(args)...);
};

}  // namespace afro::log
