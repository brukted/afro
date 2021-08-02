/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>

#include "spdlog/logger.h"

namespace afro::log {

enum class Log_level {
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

auto get_logger() -> Logger&;

auto init_log(Logger&, Log_level) -> void;

auto set_log_level(Log_level) -> void;

template <typename... Args>
constexpr auto addon_trace(const Args... args) -> void {
  afro::log::get_logger().addon_logger->trace(args...);
};
template <typename... Args>
constexpr auto addon_warn(const Args... args) -> void {
  afro::log::get_logger().addon_logger->warn(args...);
};
template <typename... Args>
constexpr auto addon_info(const Args... args) -> void {
  afro::log::get_logger().addon_logger->info(args...);
};
template <typename... Args>
constexpr auto addon_debug(const Args... args) -> void {
  afro::log::get_logger().addon_logger->debug(args...);
};
template <typename... Args>
constexpr auto addon_error(const Args... args) -> void {
  afro::log::get_logger().addon_logger->error(args...);
};

template <typename... Args>
constexpr auto addon_critical(const Args... args) -> void {
  afro::log::get_logger().addon_logger->critical(args...);
};

template <typename... Args>
constexpr auto core_trace(const Args... args) -> void {
  afro::log::get_logger().core_logger->trace(args...);
};
template <typename... Args>
constexpr auto core_warn(const Args... args) -> void {
  afro::log::get_logger().core_logger->warn(args...);
};
template <typename... Args>
constexpr auto core_info(const Args... args) -> void {
  afro::log::get_logger().core_logger->info(args...);
};
template <typename... Args>
constexpr auto core_debug(const Args... args) -> void {
  afro::log::get_logger().core_logger->debug(args...);
};
template <typename... Args>
constexpr auto core_error(const Args... args) -> void {
  afro::log::get_logger().core_logger->error(args...);
};

template <typename... Args>
constexpr auto core_critical(const Args... args) -> void {
  afro::log::get_logger().core_logger->critical(args...);
};

}  // namespace afro::log
