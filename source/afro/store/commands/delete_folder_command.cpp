#include "delete_folder_command.h"

namespace afro::store {
DeleteFolderCommand::DeleteFolderCommand(core::UUID folder_uid, Folder* parent)
    : Command("FOLDER_OP_DELETE"), folder_uid(folder_uid), parent(parent) {}

auto DeleteFolderCommand::execute() -> void {
  auto pos = std::remove_if(std::begin(parent->sub_folders),
                            std::end(parent->sub_folders),
                            [&](auto& val) { return val->uid == folder_uid; });
  if (pos != std::end(parent->sub_folders)) {
    folder = std::move(*pos);
    parent->sub_folders.erase(pos);
  }
}

auto DeleteFolderCommand::undo() -> void {
  parent->add_sub_folder(std::move(folder));
}

auto DeleteFolderCommand::redo() -> void { execute(); }
}  // namespace afro::store