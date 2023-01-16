#include "export-dialog.h"

#include "components/file-dialog.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace PS2Plus::App::Views::Configuration {
const std::vector<std::string> kDeviceList{"host:", "mc0:", "mc1:", "mass:", "mass0:", "mass1:"};

ImVec2 ExportDialog::GetPopupSize() { return ImVec2(ImGui::GetMainViewport()->Size - ImVec2(10.f, 10.f)); }

ImGuiWindowFlags ExportDialog::GetPopupFlags() { return ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; }

std::optional<std::string> ExportDialog::RenderContents() {
  std::optional<std::string> result;

  if (PS2Plus::Components::FileDialog("FileDialog", NULL, kDeviceList, selected_path_, true)) {
    ImGui::CloseCurrentPopup();

    if (selected_path_[0] != 0) {
      result = std::string(selected_path_);
      memset(selected_path_, 0, sizeof(selected_path_));
    }
  }

  return result;
}
} // namespace PS2Plus::App::Views::Configuration