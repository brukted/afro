/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "operator.h"

namespace afro::core {

using Operation = std::unique_ptr<Operator>;

struct UndoStack {
  std::vector<Operation> stack;
  int next_undo_idx = -1;
  const int max_undo;
  UndoStack(int max_undo = 0);
  /**
   * @brief Undoes the recent operations until upto @a depth .Has bound
   * checking.
   *
   * @param depth
   */
  auto undo(int depth = 1) -> void;
  /**
   * @brief Redoes the recent operations until upto @a depth .Has bound
   * checking.
   *
   * @param depth
   */
  auto redo(int depth = 1) -> void;
  [[nodiscard]] auto has_undo() const -> bool;
  [[nodiscard]] auto has_redo() const -> bool;
  auto push_operation(Operation item) -> void;
};

} // namespace afro::core