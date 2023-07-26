#include "undo_stack_impl.h"

namespace afro::undo {
auto UndoStackImpl::execute_undo() -> void {
  while (undo_depth != 0 && has_undo()) {
    operations[next_undo_idx]->undo();
    --next_undo_idx;
    --undo_depth;
  }
  undo_depth = 0;
}

auto UndoStackImpl::execute_redo() -> void {
  while (redo_depth != 0 && has_redo()) {
    operations[next_undo_idx + 1]->redo();
    ++next_undo_idx;
    --redo_depth;
  }
  redo_depth = 0;
}

auto UndoStackImpl::undo(int depth) -> void { undo_depth += depth; }

auto UndoStackImpl::redo(int depth) -> void { redo_depth += depth; }

auto UndoStackImpl::has_undo() const -> bool { return next_undo_idx != -1; }

auto UndoStackImpl::has_redo() const -> bool {
  return next_undo_idx != static_cast<int>(operations.size() - 1);
}

auto UndoStackImpl::push_operation(Operation item) -> void {
  // Removes anything undone, if there is any
  if (next_undo_idx != static_cast<int>(operations.size() - 1)) {
    operations.erase(operations.begin() + 1 + next_undo_idx, operations.end());
  }
  operations.emplace_back(std::move(item));
  next_undo_idx = static_cast<int>(operations.size() - 1);
}

auto UndoStackImpl::get_operations() const -> const std::vector<Operation>& {
  return operations;
}

auto UndoStackImpl::get_next_undo_idx() const -> int { return next_undo_idx; }
}  // namespace afro::undo
