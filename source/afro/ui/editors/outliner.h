/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#pragma once

namespace afro::core {
struct Context;
struct Folder;
struct MaterialGraph;
}  // namespace afro::core

namespace afro::ui {

struct Outliner {
  core::Context* context;
  Outliner(core::Context* context);
  auto draw(bool* p_open) -> void;

 private:
  auto main_context_menu() -> void;
  auto material_graph_menu(core::MaterialGraph* graph) -> void;
};

}  // namespace afro::ui