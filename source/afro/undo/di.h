#pragma once

#include <fruit/fruit.h>

#include "undo/data/undo_stack_impl.h"
#include "undo/interfaces/undo_stack.h"
#include "undo/ui/debug_window.h"

namespace afro::undo {
fruit::Component<UndoStack, DebugWindow> getUndoComponent() {
  return fruit::createComponent().bind<UndoStack, UndoStackImpl>();
}

}  // namespace afro::undo
