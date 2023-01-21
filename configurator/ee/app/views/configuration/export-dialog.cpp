#include "export-dialog.h"

#include "components/file-dialog.h"
#include "components/status.h"

#include <iostream>
#include <fstream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace PS2Plus::App::Views::Configuration {
const std::vector<std::string> kDeviceList{"host:", "mc0:", "mc1:", "mass:", "mass0:", "mass1:"};

ImVec2 ExportDialog::GetPopupSize() {
  switch (state_) {
  case kExportDialogSelectingPath:
    return ImVec2(ImGui::GetMainViewport()->Size - ImVec2(10.f, 10.f));

  default:
    return ImVec2(300.f, 0.f);
  }
}

ImGuiWindowFlags ExportDialog::GetPopupFlags() { return ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; }

bool WriteConfiguration(PS2Plus::App::Configuration& config, std::string path) {
  printf("Writing configuration: %s\n", path.c_str());
  std::ofstream export_stream(path, std::ios::out);
  config.Export(export_stream);

  printf("Wrote configuration: %s\n", path.c_str());
  return static_cast<bool>(export_stream);
}

void ExportDialog::RenderState_SelectingPath() {
  if (PS2Plus::Components::FileDialog("FileDialog", NULL, kDeviceList, selected_folder_, true)) {
    if (selected_folder_[0] != 0) {
      printf("Got path: %s\n", selected_folder_);
      selected_path_.clear();
      selected_path_.append(selected_folder_);
      selected_path_.append("/ps2plus-controller.toml");
      std::cout << "std::string " << selected_path_ << std::endl;
      printf("Writing to: %s\n", selected_path_.c_str());

      if (WriteConfiguration(config_, selected_path_)) {
        state_ = kExportDialogSuccessfulWrite;
      } else {
        state_ = kExportDialogFailedWrite;
      }
    } else {
      state_ = kExportDialogFinished;
    }
  }
}

void ExportDialog::RenderState_SuccessfulWrite() {
  PS2Plus::Components::StatusTextCentered("Success!", PS2Plus::Components::kStatusSuccess);
  ImGui::Spacing();
  ImGui::TextWrapped("%s", selected_path_.c_str());
  ImGui::Spacing();
  
  if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    state_ = kExportDialogFinished;
  }
}

void ExportDialog::RenderState_FailedWrite() {
  PS2Plus::Components::StatusTextCentered("Error!", PS2Plus::Components::kStatusError);
  ImGui::Spacing();
  ImGui::TextWrapped("%s", selected_path_.c_str());
  ImGui::Spacing();
  
  if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    state_ = kExportDialogFinished;
  }
}

void ExportDialog::RenderContents() {
  if (state_ == kExportDialogSelectingPath) {
    RenderState_SelectingPath();
  } else if (state_ == kExportDialogSuccessfulWrite) {
    RenderState_SuccessfulWrite();
  } else if (state_ == kExportDialogFailedWrite) {
    RenderState_FailedWrite();
  } else if (state_ == kExportDialogFinished) {
    state_ = kExportDialogSelectingPath;
    selected_folder_[0] = 0;
    selected_path_.clear();
    ImGui::CloseCurrentPopup();
  }
}
} // namespace PS2Plus::App::Views::Configuration