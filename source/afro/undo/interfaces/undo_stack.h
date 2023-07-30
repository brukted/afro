#pragma once

#include <memory>

#include "common/interfaces/command.h"

namespace afro::undo {
using Operation = std::unique_ptr<Command>;

class UndoStack {
 public:
  /**
   * @brief Adds pending requests to undo the recent operations upto @a depth .
   *
   * @param depth
   */
  virtual auto undo(int depth = 1) -> void = 0;
  /**
   * @brief Adds pending requests to redo the recent operations upto @a depth .
   *
   * @param depth
   */
  virtual auto redo(int depth = 1) -> void = 0;
  virtual auto has_undo() const -> bool = 0;
  virtual auto has_redo() const -> bool = 0;
  virtual auto enqueue(Operation item) -> void = 0;
  virtual auto execute_pending() -> void = 0;
  virtual auto get_operations() const -> const std::vector<Operation>& = 0;
  /**
   * @brief Returns the index of the next undo operation. This is used to
   * highlight the current operation in the UI.
   *
   * @return int
   */
  virtual auto get_next_undo_idx() const -> int = 0;
  virtual ~UndoStack() = default;
};
}  // namespace afro::undo
