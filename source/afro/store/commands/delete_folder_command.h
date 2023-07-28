#pragma once

#include <memory>
#include <string>

#include "common/data/uuid.h"
#include "common/interfaces/command.h"
#include "store/data/folder.h"

namespace afro::store {
struct DeleteFolderCommand : Command {
  UUID folder_uid = 0;
  Folder* parent;
  std::shared_ptr<Folder> folder;
  DeleteFolderCommand(UUID folder_uid, Folder* parent);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
  ~DeleteFolderCommand() override = default;
};
}  // namespace afro::store
