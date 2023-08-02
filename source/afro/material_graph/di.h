/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <fruit/fruit.h>

#include "material_graph/ui/material_editor.h"
#include "property/di.h"
#include "undo/di.h"

namespace afro::graph::material {

auto get_material_graph_component() -> fruit::Component<MaterialEditor> {
  return fruit::createComponent()
      .install(undo::getUndoComponent)
      .install(property::get_property_component);
}

}  // namespace afro::graph::material