#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/data/uuid.h"
#include "common/interfaces/command.h"
#include "store/data/folder.h"
#include "utils/translation.h"

namespace afro::store {
struct AddFolderCommand : Command {
  UUID folder_uid = 0;
  Folder* parent;
  std::shared_ptr<Folder> folder = nullptr;
  std::string folder_name = translate("Untitled");
  AddFolderCommand(Folder* parent);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
  auto draw() -> CommandResult override;
  ~AddFolderCommand() override = default;
};
}  // namespace afro::store
