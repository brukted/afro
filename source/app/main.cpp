#include "core/context.h"
#include "utils/constants.h"
#include "utils/log.h"
#include "utils/paths.h"
#include "utils/preferences.h"

using namespace afro;
namespace fs = std::filesystem;

auto main() -> int {
  log::init_log(log::get_logger(), log::LogLevel::trace);
  log::core_trace("Creating context");
  core::Context ctx;
  log::core_trace("Context created");
  ctx.init();
  const auto preferences_path =
      (paths::config_dir() / preferences_file_name).string();
  log::core_trace("Loading preferences from {}", preferences_path);
  Preferences::get().load_from_file(preferences_path);
  log::core_trace("Preferences loaded");

  ctx.ui_context.main_loop();
  ctx.deinit();
  return 0;
}