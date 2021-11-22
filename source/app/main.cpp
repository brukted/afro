#include <iostream>
#include <string_view>

#include "core/context.h"
#include "utils/log.h"
#include "utils/paths.h"
#include "utils/preferences.h"

using namespace afro;
namespace fs = std::filesystem;

auto main() -> int {
  constexpr std::string_view preferences_file_name = "preferences.json";
  log::init_log(log::get_logger(), log::LogLevel::trace);
  log::core_trace("Initializing context");
  core::Context ctx;
  log::core_trace("Context Initialized");
  const auto preferences_path = (paths::config_dir() / preferences_file_name).string();
  log::core_trace("Loading preferences from {}", preferences_path);
  Preferences::get().load_from_file(preferences_path);
  log::core_trace("Preferences loaded");
  ctx.ui_context.init();
  log::core_trace("Entering UI main loop");
  ctx.ui_context.main_loop();
  log::core_trace("Exited UI main loop");
  return 0;
}