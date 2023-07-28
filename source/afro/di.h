/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <fruit/fruit.h>

#include "property/di.h"
#include "store/di.h"
#include "ui/di.h"
#include "undo/di.h"

namespace afro {
auto get_root_component()
    -> fruit::Component<undo::UndoStack, undo::DebugWindow, store::Data,
                        store::Outliner, ui::Window, property::PropertyEditor> {
  return fruit::createComponent()
      .install(undo::getUndoComponent)
      .install(store::getStoreComponent)
      .install(ui::getUiComponent)
      .install(property::get_property_component);
}
}  // namespace afro