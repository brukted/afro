/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "common/data/uuid.h"
#include "property/data/property.h"

namespace afro {
class AfObject {
 private:
  UUID uuid;

 public:
  std::vector<property::Property> properties;

  AfObject(UUID uuid, std::vector<property::Property> properties)
      : uuid(uuid), properties(std::move(properties)) {}

  AfObject(std::vector<property::Property> properties)
      : AfObject(generate_uuid(), std::move(properties)) {}

  AfObject() : uuid(generate_uuid()) {}

  [[nodiscard]] auto get_uuid() const -> UUID { return uuid; }

  virtual ~AfObject() = default;
};
}  // namespace afro