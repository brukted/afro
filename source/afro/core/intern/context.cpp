/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */
#include "core/context.h"

#include "core/material_nodes.h"

namespace afro::core {

void Context::init() {
  log::core_trace("Initializing context");
  ui_context.init();
  material_engine.init();
  log::core_trace("Context Initialized");
}

void Context::deinit() {
  log::core_trace("Deinitilizing context");
  material_engine.deinit();
  ui_context.deinit();
  log::core_trace("Context deinitialized");
}
}  // namespace afro::core
