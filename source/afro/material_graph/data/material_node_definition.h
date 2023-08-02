/**
 * Copyright (c) 2023 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "property/data/property_definition.h"
#include "ui/data/icons.h"

namespace afro::graph::material {

class MaterialNode;
class MaterialEngine;
class MaterialGraph;

using MaterialNodeExecFun = std::function<void(
    MaterialEngine* engine, MaterialGraph*, MaterialNode* node)>;

class MaterialNodeDefinition {
 private:
  std::string id;
  std::string name;
  std::vector<property::PropertyDefinition> prop_definitions;
  std::string shader_code;
  ui::Icon icon;
  MaterialNodeExecFun on_execute;
  static MaterialNodeExecFun def_exec_fun;
  
 public:
  MaterialNodeDefinition(
      std::string id, std::string name,
      std::vector<property::PropertyDefinition> prop_definitions,
      std::string shader_code, ui::Icon icon,
      MaterialNodeExecFun on_execute = def_exec_fun)
      : id(std::move(id)),
        name(std::move(name)),
        prop_definitions(std::move(prop_definitions)),
        shader_code(std::move(shader_code)),
        icon(icon),
        on_execute(on_execute) {}

  [[nodiscard]] auto get_id() const -> auto& { return id; }
  [[nodiscard]] auto get_name() const -> auto& { return name; }
  [[nodiscard]] auto get_prop_definitions() const -> auto& {
    return prop_definitions;
  }
  [[nodiscard]] auto get_shader_code() const -> auto& { return shader_code; }
  [[nodiscard]] auto get_icon() const -> auto& { return icon; }
  [[nodiscard]] auto get_on_execute() -> auto& { return on_execute; }
};
}  // namespace afro::graph::material