#pragma once

#include <fruit/fruit.h>

#include <deque>

#include "undo/interfaces/undo_stack.h"

namespace afro::undo {
class UndoStackImpl : public UndoStack {
 private:
  std::vector<Operation> operations;
  std::deque<Operation> pending_operations;
  int next_undo_idx = -1;
  // Amount of undo operations to be undone on next main loop iteration.
  int undo_depth = 0;
  // Amount of redo operations to be redone on next main loop iteration.
  int redo_depth = 0;
  const int max_undo = 100;
  auto execute_undo() -> void;
  auto execute_redo() -> void;
  auto push_operation(Operation item) -> void;

 public:
  INJECT(UndoStackImpl()) {}
  auto undo(int depth) -> void override;
  auto redo(int depth) -> void override;
  auto has_undo() const -> bool override;
  auto has_redo() const -> bool override;
  auto enqueue(Operation item) -> void override;
  auto execute_pending() -> void override;
  auto get_operations() const -> const std::vector<Operation>& override;
  auto get_next_undo_idx() const -> int override;
};
}  // namespace afro::undo
