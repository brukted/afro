#include <fruit/fruit.h>

#include <memory>

#include "di.h"
#include "utils/log.h"

using namespace std;
using namespace afro;

auto main() -> int {
  log::init_log(log::get_logger(), log::LogLevel::trace);

  fruit::Injector<undo::UndoStack, undo::DebugWindow, store::Data,
                  store::Outliner, ui::Window>
      injector(get_root_component);

  auto* main_window = injector.get<ui::Window*>();

  // setup window
  main_window->startup();
  main_window->add_widget(injector.get<shared_ptr<store::Outliner>>());

  while (main_window->draw()) {
    // execute operators
  }

  main_window->shutdown();
  return 0;
}