/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include <chrono>
#include <filesystem>
#include <memory>
#include <vector>

#include "fmt/chrono.h"
#include "fmt/format.h"
#include "log.h"
#include "paths.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace afro::log {

auto get_logger() -> Logger& {
  static Logger s_logger;
  return s_logger;
}

auto init_log(Logger& logger, log_level level) -> void {
  spdlog::set_pattern("%^ [%T] %n: %v%$");
  std::vector<spdlog::sink_ptr> sinks{};

  sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

  std::time_t time_now = std::time(nullptr);
  auto log_file_name = fmt::format("{:%Y_%m_%d-%H_%M_%S} log.txt", fmt::localtime(time_now));
  auto log_file = (paths::log_dir() / log_file_name).u8string();
  sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file));

  logger.addon_logger = std::make_shared<spdlog::logger>("addon", sinks.begin(), sinks.end());
  logger.core_logger = std::make_shared<spdlog::logger>("core", sinks.begin(), sinks.end());
  logger.gl_logger = std::make_shared<spdlog::logger>("gl", sinks.begin(), sinks.end());

  set_log_level(level);
}

auto set_log_level(log_level level) -> void {
  Logger& s_logger = get_logger();
  s_logger.addon_logger->set_level((spdlog::level::level_enum)((int)level));
  s_logger.core_logger->set_level((spdlog::level::level_enum)((int)level));
  s_logger.gl_logger->set_level((spdlog::level::level_enum)((int)level));
}

}  // namespace afro::log