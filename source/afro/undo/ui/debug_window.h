#pragma once

#include <fruit/fruit.h>

#include "ui/interfaces/widget.h"
#include "undo/interfaces/undo_stack.h"

namespace afro::undo {
class DebugWindow : public ui::Widget {
 private:
  UndoStack* undo_stack;

 public:
  INJECT(DebugWindow(UndoStack* undo_stack)) : undo_stack(undo_stack){};
  auto draw() -> void override;
};
}  // namespace afro::undo
