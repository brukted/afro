#include "add_folder_command.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <cstddef>
#include <utility>

#include "core/data/uuid.h"

namespace afro::store {
using afro::core::CommandResult;

store::AddFolderCommand::AddFolderCommand(Folder* parent)
    : Command("FOLDER_OP_CREATE"), parent(parent) {}

auto store::AddFolderCommand::execute() -> void {
  auto new_folder =
      std::make_unique<Folder>(core::generate_uuid(), folder_name);
  folder_uid = new_folder->uid;
  parent->add_item(std::move(new_folder));
}

auto store::AddFolderCommand::undo() -> void {
  auto pos = std::remove_if(std::begin(parent->sub_folders),
                            std::end(parent->sub_folders),
                            [&](auto& val) { return val->uid == folder_uid; });
  if (pos != std::end(parent->sub_folders)) {
    folder = std::move(*pos);
    parent->sub_folders.erase(pos);
    parent->items.erase(
        std::remove_if(std::begin(parent->items), std::end(parent->items),
                       [&](auto& val) { return val->get_uid() == folder_uid; }),
        std::end(parent->items));
  }
}

auto store::AddFolderCommand::redo() -> void {
  parent->add_sub_folder(std::move(folder));
}

auto store::AddFolderCommand::draw() -> core::CommandResult {
  auto result = CommandResult::DRAWING_UI;
  ImGui::OpenPopup(translate("Add Folder"));
  if (ImGui::BeginPopupModal(translate("Add Folder"))) {
    ImGui::InputText(translate("Name"), &folder_name);
    if (ImGui::Button(translate("Create"))) {
      ImGui::CloseCurrentPopup();
      result = CommandResult::FINISHED_UI;
    }
    ImGui::EndPopup();
  }
  return result;
}

}  // namespace afro::store