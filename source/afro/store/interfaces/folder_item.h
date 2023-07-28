#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "core/data/uuid.h"

namespace afro::ui {
enum class Icon;
}

namespace afro::store {
class FolderItem {
 public:
  virtual auto get_label() -> std::string_view = 0;
  virtual auto get_icon() -> ui::Icon = 0;
  virtual auto get_uid() -> core::UUID = 0;
  virtual auto get_sub_items()
      -> const std::vector<std::shared_ptr<FolderItem>>& = 0;
  virtual ~FolderItem() = default;
};
}  // namespace afro::store
