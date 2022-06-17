/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <stdexcept>
#include <string_view>

namespace afro::core {

enum class OperatorResult { DRAWING_UI, CANCELED, FINISHED_UI };

class OperatorError : public std::runtime_error {
 public:
  OperatorError(const std::string &msg) : std::runtime_error(msg){};
};

struct Operator {
  const std::string_view id_name;
  // Indicates whether the state before the operation can be recovered
  const bool can_undo;
  Operator(std::string_view id_name, bool can_undo = false);
  virtual auto execute() -> void = 0;
  virtual auto undo() -> void = 0;
  virtual auto redo() -> void = 0;
  /**
   * @brief Draws the UI of the operator.
   *
   * @return DrawResult in the set {DRAWING_UI, FINISHED_UI, CANCELED}
   */
  virtual auto draw() -> OperatorResult { return OperatorResult::FINISHED_UI; };
  Operator(Operator &&) = default;
  Operator(Operator &) = delete;
  auto operator=(Operator &&) -> Operator & = delete;
  auto operator=(Operator &) -> Operator & = delete;
  virtual ~Operator() = default;
};
}  // namespace afro::core