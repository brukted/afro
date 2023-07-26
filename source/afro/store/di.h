#pragma once

#include <fruit/fruit.h>

#include "store/data/data.h"
#include "store/ui/outliner.h"

namespace afro::store {
fruit::Component<store::Data, store::Outliner> getStoreComponent() {
  return fruit::createComponent();
}
}  // namespace afro::store
