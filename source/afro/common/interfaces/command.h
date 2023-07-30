#pragma once

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace afro {

enum class CommandResult { DRAWING_UI, CANCELED, FINISHED_UI };

class CommandError : public std::runtime_error {
 public:
  CommandError(const std::string &msg) : std::runtime_error(msg){};
};

class Command {
 public:
  const std::string_view id_name;
  // Indicates whether the state before the operation can be recovered
  const bool can_undo;

  Command(std::string_view id_name, bool can_undo = true)
      : id_name(id_name), can_undo(can_undo){};

  virtual auto execute() -> void = 0;
  virtual auto undo() -> void = 0;
  virtual auto redo() -> void { execute(); };
  /**
   * @brief Draws the UI of the command.
   *
   * @return DrawResult in the set {DRAWING_UI, FINISHED_UI, CANCELED}
   */
  virtual auto draw() -> CommandResult { return CommandResult::FINISHED_UI; };
  Command(Command &&) = default;
  Command(Command &) = delete;
  auto operator=(Command &&) -> Command & = delete;
  auto operator=(Command &) -> Command & = delete;
  virtual ~Command() = default;
};
}  // namespace afro
