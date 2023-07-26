#pragma once

#include <fruit/fruit.h>

#include "undo/data/undo_stack_impl.h"
#include "undo/interfaces/undo_stack.h"

namespace afro::undo {
fruit::Component<UndoStack> getUndoComponent() {
  return fruit::createComponent().bind<UndoStack, UndoStackImpl>();
}

}  // namespace afro::undo
