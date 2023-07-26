#pragma once

#include <fruit/fruit.h>

#include "data/main_window.h"
#include "interfaces/window.h"

namespace afro::ui {
fruit::Component<Window> getUiComponent() {
  return fruit::createComponent().bind<Window, MainWindow>();
}
}  // namespace afro::ui
