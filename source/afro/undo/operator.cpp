/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "operator.h"

namespace afro::core {
Operator::Operator(std::string_view id_name, bool can_undo)
    : id_name(id_name), can_undo(can_undo) {}
} // namespace afro::core