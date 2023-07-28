/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/data/uuid.h"
#include "store/interfaces/folder_item.h"

namespace afro::store {
struct Folder : public FolderItem {
  const UUID uid;
  std::string name;
  Folder* parent_folder = nullptr;
  std::vector<std::shared_ptr<Folder>> sub_folders;
  std::vector<std::shared_ptr<FolderItem>> items;
  Folder(UUID uid, std::string name);
  Folder(UUID uid, std::string name, Folder* parent_folder);

  auto get_label() -> std::string_view override;
  auto get_icon() -> ui::Icon override;
  auto get_uid() -> UUID override;
  auto get_sub_items()
      -> const std::vector<std::shared_ptr<FolderItem>>& override;

  auto add_sub_folder(std::shared_ptr<Folder> folder) -> void;
  auto add_item(std::shared_ptr<FolderItem> item) -> void;
};
}  // namespace afro::store