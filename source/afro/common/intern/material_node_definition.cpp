/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "core/material_node_definition.h"

namespace afro::core {
MaterialNodeDefinition::MaterialNodeDefinition(
    const std::string_view &ui_name, const std::string_view &tooltip,
    ui::Icon icon,
    std::vector<std::unique_ptr<PropertyDefinition>> prop_definitions,
    std::vector<MaterialSocketDefinition> input_socket_definitions,
    std::vector<MaterialSocketDefinition> output_socket_definitions,
    ExecuteNodeFunc execute_node_func)
    : ui_name(ui_name),
      tooltip(tooltip),
      icon(icon),
      prop_definitions(std::move(prop_definitions)),
      input_socket_definitions(std::move(input_socket_definitions)),
      output_socket_definitions(std::move(output_socket_definitions)),
      execute_node_func(execute_node_func) {}
}  // namespace afro::core