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

struct Command {
  const std::string_view id_name;
  const std::function<void()> on_execute;
  const std::function<void()> on_undo;
  const std::function<void()> on_redo;
  // Indicates whether the state before the operation can be recovered
  const bool can_undo;

  Command(
      std::string_view id_name, bool can_undo = false,
      std::function<void()> on_execute = []() -> void {},
      std::function<void()> on_undo = []() -> void {},
      std::function<void()> on_redo = []() -> void {})
      : id_name(id_name),
        on_execute(on_execute),
        on_undo(on_undo),
        on_redo(on_redo),
        can_undo(can_undo){};

  virtual auto execute() -> void = 0;
  virtual auto undo() -> void = 0;
  virtual auto redo() -> void = 0;
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
