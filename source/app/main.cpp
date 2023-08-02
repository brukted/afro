#include <fruit/fruit.h>

#include <memory>

#include "di.h"
#include "property/data/property_definition.h"
#include "utils/log.h"

using namespace std;
using namespace afro;

auto main() -> int {
  log::init_log(log::get_logger(), log::LogLevel::trace);

  fruit::Injector<undo::UndoStack, undo::DebugWindow, store::Data,
                  store::Outliner, ui::Window, property::PropertyEditor,
                  graph::material::MaterialEditor>
      injector(get_root_component);

  auto* main_window = injector.get<ui::Window*>();

  // setup window
  main_window->startup();
  main_window->add_widget(injector.get<shared_ptr<store::Outliner>>());
  main_window->add_widget(injector.get<shared_ptr<property::PropertyEditor>>());
  main_window->add_widget(
      injector.get<shared_ptr<graph::material::MaterialEditor>>());

  auto graph = make_shared<graph::material::MaterialGraph>();
  injector.get<shared_ptr<graph::material::MaterialEditor>>()->set_graph(graph);

  auto undo = injector.get<shared_ptr<undo::UndoStack>>();
  while (main_window->draw()) {
    undo->execute_pending();
  }

  main_window->shutdown();
  return 0;
}