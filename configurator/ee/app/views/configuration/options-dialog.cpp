#include "options-dialog.h"

namespace PS2Plus::App::Views::Configuration {
std::optional<OptionsDialogResult> OptionsDialog::RenderContents() {
  std::optional<OptionsDialogResult> result;

  if (ImGui::Selectable("Save")) {
    result = kOptionsResultSave;
  }

  if (ImGui::Selectable("Reset")) {
    result = kOptionsResultReset;
  }

  ImGui::Separator();

  if (ImGui::Selectable("Import")) {
    result = kOptionsResultImport;
  }

  if (ImGui::Selectable("Export")) {
    result = kOptionsResultExport;
  }

  if (ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight)) {
    ImGui::CloseCurrentPopup();
  }

  return result;
}

ImVec2 OptionsDialog::GetPopupSize() { return ImVec2(150.f, 0); }

ImGuiWindowFlags OptionsDialog::GetPopupFlags() {
  return ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
}
} // namespace PS2Plus::App::Views::Configuration