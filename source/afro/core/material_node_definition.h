/**
 * Copyright (c) 2022 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once
#include <string_view>

#include "core/uuid.h"
#include "material_engine.h"
#include "property.h"
#include "ui/icons.h"
#include "utils/translation.h"

namespace afro::core {
class MaterialEngineContext;

struct MaterialSocketDefinition {
  const std::string_view ui_name{_("socket")};
  const MaterialSocketType type{MaterialSocketType::color};

  MaterialSocketDefinition(const std::string_view &ui_name,
                           const MaterialSocketType type)
      : ui_name(ui_name), type(type) {}
};

using ExecuteNodeFunc =
    void (*)(PropsMap &common_props, PropsMap &specific_props,
             std::map<UUID, OutputSocketBuffer> *node_gpu_buffers,
             MaterialEngineContext *context);

struct MaterialNodeDefinition {
  std::string_view ui_name;
  std::string_view tooltip;
  ui::Icon icon;
  std::vector<std::unique_ptr<PropertyDefinition>> prop_definitions;

  std::vector<MaterialSocketDefinition> input_socket_definitions;
  std::vector<MaterialSocketDefinition> output_socket_definitions;

  ExecuteNodeFunc execute_node_func;

  MaterialNodeDefinition(
      const std::string_view &ui_name, const std::string_view &tooltip,
      ui::Icon icon,
      std::vector<std::unique_ptr<PropertyDefinition>> prop_definitions,
      std::vector<MaterialSocketDefinition> input_socket_definitions,
      std::vector<MaterialSocketDefinition> output_socket_definitions,
      ExecuteNodeFunc execute_node_func);
};
}  // namespace afro::core
