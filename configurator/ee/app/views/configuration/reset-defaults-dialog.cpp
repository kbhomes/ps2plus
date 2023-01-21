#include "reset-defaults-dialog.h"

namespace PS2Plus::App::Views::Configuration {
ImVec2 ResetDefaultsDialog::GetPopupSize() {
  return ImVec2(350.f, 0.f);
}

ImGuiWindowFlags ResetDefaultsDialog::GetPopupFlags() {
  return ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
}

void ResetDefaultsDialog::RenderContents() {
  ImGui::TextWrapped("Reset configuration values back to their default values?");
  ImGui::Spacing();

  if (ImGui::BeginTable("Actions", 2)) {
    ImGui::TableNextColumn();
    if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 0.f))) {
      ResetConfiguration();
      ImGui::CloseCurrentPopup();
    }
    
    ImGui::TableNextColumn();
    if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvail().x, 0.f))) {
      ImGui::CloseCurrentPopup();
    }
    
    ImGui::EndTable();
  }
}

void ResetDefaultsDialog::ResetConfiguration() {
  config_.Reset();
}
} // namespace PS2Plus::App::Views::Configuration