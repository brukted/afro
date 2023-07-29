/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <unordered_map>

#include "common/data/uuid.h"

namespace afro::graph {
/**
 * @brief A bidirectional map between UUIDs and imnodes ids.
 *
 */
struct IDMap {
 private:
  int last_i = 0;
  std::unordered_map<UUID, int> map1;
  std::unordered_map<int, UUID> map2;

 public:
  inline auto get_uuid(int imnodes_id) { return map2.at(imnodes_id); };
  inline auto get_imnodes_id(UUID uuid) { return map1.at(uuid); };
  inline auto create_or_get_imnodes_id(UUID uuid) {
    if (map1.contains(uuid)) {
      return map1.at(uuid);
    }
    ++last_i;
    map1[uuid] = last_i;
    map2[last_i] = uuid;
    return last_i;
  }
  inline auto clear() -> void {
    map1.clear();
    map2.clear();
  };
};
}  // namespace afro::graph