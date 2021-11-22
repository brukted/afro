/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "folder_ops.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <cstddef>
#include <utility>

#include "core/folder.h"
#include "undo/operator.h"
#include "utils/log.h"
#include "utils/translation.h"

namespace afro::core {
AddFolder::AddFolder(Folder* parent) : Operator("FOLDER_OP_CREATE"), parent(parent) {}

auto AddFolder::execute() -> void {
  auto new_folder = std::make_unique<Folder>(generate_uuid(), folder_name);
  folder_uid = new_folder->uid;
  parent->add_item(std::move(new_folder));
}

auto AddFolder::undo() -> void {
  auto pos = std::remove_if(std::begin(parent->sub_folders), std::end(parent->sub_folders),
                            [&](auto& val) { return val->uid == folder_uid; });
  if (pos != std::end(parent->sub_folders)) {
    folder = std::move(*pos);
    parent->sub_folders.erase(pos);
  }
}

auto AddFolder::redo() -> void { parent->add_item(std::move(folder)); }

auto AddFolder::draw() -> OperatorResult {
  OperatorResult result = OperatorResult::DRAWING_UI;
  ImGui::OpenPopup(translate("Add Folder"));
  if (ImGui::BeginPopupModal(translate("Add Folder"))) {
    ImGui::InputText(translate("Name"), &folder_name);
    if (ImGui::Button(translate("Create"))) {
      ImGui::CloseCurrentPopup();
      result = OperatorResult::FINISHED_UI;
    }
    ImGui::EndPopup();
  }
  return result;
}

DeleteFolder::DeleteFolder(UUID folder_uid, Folder* parent)
    : Operator("FOLDER_OP_DELETE"), folder_uid(folder_uid), parent(parent) {}

auto DeleteFolder::execute() -> void {
  auto pos = std::remove_if(std::begin(parent->sub_folders), std::end(parent->sub_folders),
                            [&](auto& val) { return val->uid == folder_uid; });
  if (pos != std::end(parent->sub_folders)) {
    folder = std::move(*pos);
    parent->sub_folders.erase(pos);
  }
}

auto DeleteFolder::undo() -> void { parent->add_item(std::move(folder)); }

auto DeleteFolder::redo() -> void { execute(); }

}  // namespace afro::core