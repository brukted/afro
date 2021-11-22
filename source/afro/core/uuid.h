/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <cstdint>

namespace afro::core {

using UUID = uint64_t;

auto generate_uuid() -> UUID;

}  // namespace afro::core
