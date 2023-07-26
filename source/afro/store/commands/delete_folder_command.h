#pragma once

#include <memory>
#include <string>

#include "core/data/uuid.h"
#include "core/interfaces/command.h"
#include "store/data/folder.h"

namespace afro::store {
struct DeleteFolderCommand : core::Command {
  core::UUID folder_uid = 0;
  Folder* parent;
  std::shared_ptr<Folder> folder;
  DeleteFolderCommand(core::UUID folder_uid, Folder* parent);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
  ~DeleteFolderCommand() override = default;
};
}  // namespace afro::store
