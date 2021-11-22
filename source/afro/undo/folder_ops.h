/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include <core/uuid.h>

#include <memory>

#include "operator.h"
#include "utils/translation.h"

namespace afro::core {

struct Context;
struct Folder;

struct AddFolder : Operator {
  UUID folder_uid = 0;
  Folder* parent;
  std::unique_ptr<Folder> folder;
  std::string folder_name{translate("Untitled")};
  AddFolder(Folder* parent);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
  auto draw() -> OperatorResult override;
  ~AddFolder() override = default;
};

struct DeleteFolder : Operator {
  UUID folder_uid = 0;
  Folder* parent;
  std::unique_ptr<Folder> folder;
  DeleteFolder(UUID folder_uid, Folder* parent);
  auto execute() -> void override;
  auto undo() -> void override;
  auto redo() -> void override;
  ~DeleteFolder() override = default;
};

}  // namespace afro::core