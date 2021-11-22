/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "undo.h"

namespace afro::core {
UndoStack::UndoStack(int max_undo) : max_undo(max_undo) {}

auto UndoStack::undo(int depth) -> void {
  while (depth != 0 && has_undo()) {
    stack[next_undo_idx]->undo();
    --next_undo_idx;
    --depth;
  }
}

auto UndoStack::redo(int depth) -> void {
  while (depth != 0 && has_redo()) {
    stack[next_undo_idx + 1]->redo();
    ++next_undo_idx;
    --depth;
  }
}

auto UndoStack::has_undo() const -> bool { return next_undo_idx != -1; }

auto UndoStack::has_redo() const -> bool {
  return next_undo_idx != static_cast<int>(stack.size() - 1);
}

auto UndoStack::push_operation(Operation item) -> void {
  // Removes anything undone, if there is any
  if (next_undo_idx != static_cast<int>(stack.size() - 1)) {
    stack.erase(stack.begin() + 1 + next_undo_idx, stack.end());
  }
  stack.emplace_back(std::move(item));
  next_undo_idx = static_cast<int>(stack.size() - 1);
}

}  // namespace afro::core