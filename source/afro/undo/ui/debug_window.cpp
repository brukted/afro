#include "debug_window.h"

#include <imgui.h>

namespace afro::undo {
auto DebugWindow::draw() -> void {
  if (ImGui::Begin("Debug Undo")) {
    if (ImGui::BeginListBox("Undo Stack")) {
      int i = -1;
      for (const auto& undo_item : undo_stack->get_operations()) {
        ++i;
        if (undo_stack->get_next_undo_idx() == i) {
          ImGui::BulletText("%s", undo_item->id_name.data());
          continue;
        }
        ImGui::TextUnformatted(undo_item->id_name.data());
      }
      ImGui::EndListBox();
    }
    if (ImGui::Button("Undo") && undo_stack->has_undo()) {
      undo_stack->undo();
    }
    if (ImGui::Button("Redo") && undo_stack->has_redo()) {
      undo_stack->redo();
    }
    ImGui::End();
  }
}
}  // namespace afro::undo
