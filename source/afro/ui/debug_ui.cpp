/**
 * Copyright 2021 The Afro Authors. All rights reserved.
 * Use of this source code is governed by the GPL-2.0 license that can be
 * found in the LICENSE file.
 */

#include "debug_ui.h"

#include <imgui.h>

#include "core/context.h"

namespace afro::ui {
auto debug_undo(core::Context* context) -> void {
  if (ImGui::Begin("Debug Undo")) {
    if (ImGui::BeginListBox("Undo Stack")) {
      int i = -1;
      for (const auto& undo_item : context->undo_stack.stack) {
        ++i;
        if (context->undo_stack.next_undo_idx == i) {
          ImGui::BulletText("%s", undo_item->id_name.data());
          continue;
        }
        ImGui::TextUnformatted(undo_item->id_name.data());
      }
      ImGui::EndListBox();
    }
    if (ImGui::Button("Undo") && context->undo_stack.has_undo()) {
      ++(context->next_undo_depth);
    }
    if (ImGui::Button("Redo") && context->undo_stack.has_redo()) {
      ++(context->next_redo_depth);
    }
    ImGui::End();
  }
}

}  // namespace afro::ui