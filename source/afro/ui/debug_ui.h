/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

namespace afro::core {
struct Context;
}

namespace afro::ui {
auto debug_undo(core::Context* context) -> void;
}