/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */
#pragma once

#include <memory>
#include <queue>

#include "data.h"
#include "ui/ui_main.h"
#include "undo/undo.h"

namespace afro::core {

/**
 * @brief Afro's main context. Holds the state of afro's instance.
 *
 */
struct Context {
  ui::UiContext ui_context{this};
  Data data{};
  UndoStack undo_stack;
  // Queue of commands to be executed.
  std::queue<std::unique_ptr<Operator>> operator_queue;
  int next_undo_depth = 0;
  int next_redo_depth = 0;
  template <typename T, typename... Args>
  auto queue_operation(Args... args) -> void {
    operator_queue.push(std::make_unique<T>(args...));
  }
};

}  // namespace afro::core
