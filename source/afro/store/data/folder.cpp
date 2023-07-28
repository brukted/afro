/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "folder.h"

#include <stdexcept>
#include <utility>

#include "ui/data/icons.h"

namespace afro::store {
using afro::UUID;

Folder::Folder(UUID uid, std::string name) : uid(uid), name(std::move(name)) {}

Folder::Folder(UUID uid, std::string name, Folder* parent_folder)
    : uid(uid), name(std::move(name)), parent_folder(parent_folder) {}

auto Folder::get_label() -> std::string_view { return name; }

auto Folder::get_icon() -> ui::Icon { return ui::Icon::FOLDER; }

auto Folder::get_uid() -> UUID { return uid; }

auto Folder::get_sub_items()
    -> const std::vector<std::shared_ptr<FolderItem>>& {
  return items;
}
auto Folder::add_sub_folder(std::shared_ptr<Folder> folder) -> void {
  sub_folders.push_back(folder);
  items.push_back(folder);
}

auto Folder::add_item(std::shared_ptr<FolderItem> item) -> void {
  items.push_back(item);
}
}  // namespace afro::store
