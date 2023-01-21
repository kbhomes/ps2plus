#include "import-dialog.h"

#include "components/file-dialog.h"
#include "components/status.h"

#include <fstream>
#include <iostream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace PS2Plus::App::Views::Configuration {
const std::vector<std::string> kDeviceList{"host:", "mc0:", "mc1:", "mass:", "mass0:", "mass1:"};

ImVec2 ImportDialog::GetPopupSize() {
  switch (state_) {
  case kImportDialogSelectingPath:
    return ImVec2(ImGui::GetMainViewport()->Size - ImVec2(10.f, 10.f));

  default:
    return ImVec2(300.f, 0.f);
  }
}

ImGuiWindowFlags ImportDialog::GetPopupFlags() { return ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; }

bool ReadConfiguration(std::string path, PS2Plus::App::Configuration& target_config) {
  printf("Reading configuration: %s\n", path.c_str());
  std::ifstream import_stream(path, std::ios::in);
  target_config = PS2Plus::App::Configuration::Import(import_stream);
  return true;
}

void ImportDialog::RenderState_SelectingPath() {
  if (PS2Plus::Components::FileDialog("FileDialog", NULL, kDeviceList, selected_path_, false)) {
    if (selected_path_[0] != 0) {
      printf("Got path: %s\n", selected_path_);
      if (ReadConfiguration(selected_path_, target_config_)) {
        state_ = kImportDialogSuccessfulRead;
      } else {
        state_ = kImportDialogFailedRead;
      }
    } else {
      state_ = kImportDialogCancelledFinished;
    }
  }
}

void ImportDialog::RenderState_SuccessfulRead() {
  PS2Plus::Components::StatusTextCentered("Success!", PS2Plus::Components::kStatusSuccess);
  ImGui::Spacing();
  ImGui::TextWrapped("%s", selected_path_);
  ImGui::Spacing();

  if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    state_ = kImportDialogSuccessfulFinished;
  }
}

void ImportDialog::RenderState_FailedRead() {
  PS2Plus::Components::StatusTextCentered("Error!", PS2Plus::Components::kStatusError);
  ImGui::Spacing();
  ImGui::TextWrapped("%s", selected_path_);
  ImGui::Spacing();

  if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    state_ = kImportDialogFailedFinished;
  }
}

void ImportDialog::RenderContents() {
  if (state_ == kImportDialogSelectingPath) {
    RenderState_SelectingPath();
  } else if (state_ == kImportDialogSuccessfulRead) {
    RenderState_SuccessfulRead();
  } else if (state_ == kImportDialogFailedRead) {
    RenderState_FailedRead();
  } else if (state_ == kImportDialogSuccessfulFinished || state_ == kImportDialogFailedFinished ||
             state_ == kImportDialogCancelledFinished) {
    state_ = kImportDialogSelectingPath;
    selected_path_[0] = 0;
    ImGui::CloseCurrentPopup();
  }
}
} // namespace PS2Plus::App::Views::Configuration