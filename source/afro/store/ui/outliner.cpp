#include "outliner.h"

#include <imgui.h>

#include <functional>

#include "ui/data/icons.h"
#include "utils/translation.h"

namespace afro::store {
auto Outliner::draw() -> void {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(translate("Editors"))) {
      ImGui::MenuItem(translate("Outliner"), nullptr, &show);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (!show) return;

  if (!ImGui::Begin(translate("Outliner"), &show)) {
    ImGui::End();
    return;
  }
  main_context_menu();
  std::function<void(FolderItem *)> draw_folder =
      [&](FolderItem *item) -> void {
    if (ImGui::TreeNode((void *)item, "%s %s",
                        ui::icon_code_point(item->get_icon()),
                        item->get_label().data())) {
      for (auto &sub_item : item->get_sub_items()) {
        draw_folder(sub_item.get());
      }
      ImGui::TreePop();
    }
  };
  draw_folder(dynamic_cast<FolderItem *>(&(data->root_folder)));
  ImGui::End();
}

auto Outliner::main_context_menu() -> void {
  if (ImGui::BeginPopupContextWindow()) {
    if (ImGui::BeginMenu(translate("New"))) {
      // draw_operator<AddFunctionGraph>(translate("Function Graph"),context,
      // Args args...)
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(translate("Import"))) {
      // draw_operator<ImportImageTexture>(translate("Image
      // Texture"),context,Args args...)
      ImGui::EndMenu();
    }
    ImGui::Separator();
    // draw_operator<Paste>(translate("Paste"),context,Args args...)
    ImGui::EndPopup();
  }
}
}  // namespace afro::store
