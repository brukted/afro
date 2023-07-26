#include <fruit/fruit.h>

#include <memory>

#include "store/di.h"
#include "ui/di.h"
#include "utils/log.h"

using namespace std;
using namespace afro;

auto main() -> int {
  log::init_log(log::get_logger(), log::LogLevel::trace);

  fruit::Injector<ui::Window> injector(ui::getUiComponent);
  auto mainWindow = injector.get<ui::Window*>();
  mainWindow->startup();

  fruit::Injector<store::Data, store::Outliner> storeInjector(
      store::getStoreComponent);
  mainWindow->add_widget(storeInjector.get<shared_ptr<store::Outliner>>());

  while (mainWindow->draw()) {
    // execute operators
  }

  mainWindow->shutdown();
  return 0;
}