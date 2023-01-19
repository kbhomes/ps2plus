#include "app.h"

#include "components/file-dialog.h"
#include "components/icons.h"
#include "components/loading-progress.h"
#include "components/status.h"
#include "components/wizard.h"
#include <cstring>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <util/firmware-update.h>
#include <util/versions.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#define MAX_PATH 255

namespace PS2Plus::App::Views {
const ImU32 COLOR_SUCCESS = IM_COL32(0x00, 0xFF, 0x00, 0x80);
const ImU32 COLOR_ERROR = IM_COL32(0xFF, 0x00, 0x00, 0x80);
const ImU32 COLOR_WARNING = IM_COL32(0xFF, 0xFF, 0x00, 0x80);

std::vector<std::string> device_list = {"host:", "mc0:", "mc1:", "mass:", "mass0:", "mass1:"};
char path[MAX_PATH];
int current_step = 0;
FirmwareUpdate *firmware_update;

enum FirmwareUpdateType { NoChange, Upgrade, Downgrade };

void FirmwareWizardStep_Update() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  
  bool should_open_file_dialog = false;
  if (ImGui::BeginTable("UpdateButtonTable", 3)) {
    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Button", ImGuiTableColumnFlags_WidthFixed);

    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Update:");

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputText("##UpdatePath", path, IM_ARRAYSIZE(path), ImGuiInputTextFlags_ReadOnly);

    ImGui::TableNextColumn();
    should_open_file_dialog = ImGui::Button("Browse");

    ImGui::EndTable();
  }

  if (should_open_file_dialog) {
    ImGui::OpenPopup("Choose firmware update file");
  }

  // Display the file chooser popup
  ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 10, ImGui::GetIO().DisplaySize.y - 10));
  if (ImGui::BeginPopupModal("Choose firmware update file", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
    if (PS2Plus::Components::FileDialog("FileDialog-FirmwareUpdate", NULL, device_list, path)) {
      printf("Got path: %s\n", path);
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::Separator();

  ImGui::TextWrapped("Manually update PS2+ firmware:");
  ImGui::Bullet();
  ImGui::TextWrapped("Download the latest PS2+ firmware update file from <TBD>");
  ImGui::Indent();
  ImGui::Bullet();
  ImGui::TextWrapped("Microcontroller: %s", state.current_controller()->versions().microcontroller);
  ImGui::Bullet();
  ImGui::TextWrapped("Update Only (not Bootloader or Combined)");
  ImGui::Unindent();

  // Hack: Create a child window to move the action buttons to the bottom of the pane
  ImGui::BeginChild("ChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 8.f), false, ImGuiWindowFlags_NoNav);
  ImGui::EndChild();

  ImGui::Separator();

  if (ImGui::Button("Test")) {
    ImGui::OpenPopup("TestSelectFirmwareUpdate");
  }
  ImGui::SameLine();
  if (ImGui::BeginPopup("TestSelectFirmwareUpdate")) {
    for (auto testFilename : FirmwareUpdate::GetTestFilenames()) {
      if (ImGui::Selectable(testFilename.c_str())) {
        // Read the test firmware
        firmware_update = new FirmwareUpdate(testFilename);
        PS2Plus::Components::WizardNext();
      }
    }
    ImGui::EndPopup();
  }

  ImVec2 actionButtonSize = ImVec2(100, 0);
  ImGui::Dummy(ImVec2());
  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - actionButtonSize.x);
  bool hasPath = path[0] != 0;
  ImGui::BeginDisabled(!hasPath);
  if (ImGui::Button("Continue", actionButtonSize)) {
    // Read the firmware
    firmware_update = new FirmwareUpdate(path);
    PS2Plus::Components::WizardNext();
  }

  ImGui::EndDisabled();
}

void FirmwareWizardStep_Review() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();

  uint64_t version_firmware_current = state.current_controller()->versions().firmware;
  uint64_t version_firmware_update = firmware_update->GetFirmwareVersion();
  FirmwareUpdateType version_firmware_update_type = (version_firmware_update > version_firmware_current)   ? Upgrade
                                                 : (version_firmware_update < version_firmware_current) ? Downgrade
                                                                                                    : NoChange;

  char *version_microcontroller_current = state.current_controller()->versions().microcontroller;
  std::string version_microcontroller_update = firmware_update->GetMicrocontrollerVersion();
  bool version_microcontroller_mismatch = std::strcmp(version_microcontroller_update.c_str(), version_microcontroller_current) != 0;

  uint8_t version_configuration_current = state.current_controller()->versions().configuration;
  uint8_t version_configuration_update = firmware_update->GetConfigurationVersion();
  bool version_configuration_mismatch = (version_configuration_current != version_configuration_update);

  static bool acceptIssues = false;
  bool anyIssues = (version_firmware_update_type != Upgrade) || version_microcontroller_mismatch || version_configuration_mismatch;

  if (ImGui::BeginTable("FirmwareUpdateVersions", 3, ImGuiTableFlags_Borders)) {
    ImGui::TableSetupColumn("Version", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Current", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Update", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Firmware");
    ImGui::TableNextColumn();
    ImGui::Text("%s", format_version(state.current_controller()->versions().firmware).c_str());
    ImGui::TableNextColumn();
    if (version_firmware_update_type == Upgrade)
      PS2Plus::Components::IconArrow(ImGui::GetFontSize(), COLOR_SUCCESS, ImGuiDir_Up);
    else if (version_firmware_update_type == Downgrade)
      PS2Plus::Components::IconArrow(ImGui::GetFontSize(), COLOR_ERROR, ImGuiDir_Down);
    else if (version_firmware_update_type == NoChange)
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
    ImGui::SameLine();
    ImGui::Text("%s", format_version(firmware_update->GetFirmwareVersion()).c_str());

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Microcontroller");
    ImGui::TableNextColumn();
    ImGui::Text("%s", state.current_controller()->versions().microcontroller);
    ImGui::TableNextColumn();
    if (version_microcontroller_mismatch)
      PS2Plus::Components::IconError(ImGui::GetFontSize(), COLOR_ERROR);
    else
      PS2Plus::Components::IconCheckmark(ImGui::GetFontSize(), COLOR_SUCCESS);
    ImGui::SameLine();
    ImGui::Text("%s", firmware_update->GetMicrocontrollerVersion().c_str());

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Configuration");
    ImGui::TableNextColumn();
    ImGui::Text("%d", state.current_controller()->versions().configuration);
    ImGui::TableNextColumn();
    if (version_configuration_mismatch)
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
    else
      PS2Plus::Components::IconCheckmark(ImGui::GetFontSize(), COLOR_SUCCESS);
    ImGui::SameLine();
    ImGui::Text("%d", firmware_update->GetConfigurationVersion());

    ImGui::EndTable();
  }

  if (anyIssues) {
    ImGui::Separator();
    ImGui::Text("Issues:");

    if (version_firmware_update_type == Downgrade) {
      ImGui::Bullet();
      PS2Plus::Components::IconArrow(ImGui::GetFontSize(), COLOR_ERROR, ImGuiDir_Down);
      ImGui::SameLine();
      ImGui::TextWrapped("Firmware will downgrade the PS2+");
    } else if (version_firmware_update_type == NoChange) {
      ImGui::Bullet();
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
      ImGui::SameLine();
      ImGui::TextWrapped("Firmware may already be on the PS2+");
    }

    if (version_microcontroller_mismatch) {
      ImGui::Bullet();
      PS2Plus::Components::IconError(ImGui::GetFontSize(), COLOR_ERROR);
      ImGui::SameLine();
      ImGui::TextWrapped("Firmware is not for this microcontroller and may damage the PS2+");
    }

    if (version_configuration_mismatch) {
      ImGui::Bullet();
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
      ImGui::SameLine();
      ImGui::TextWrapped("PS2+ configuration will be reset after update");
    }
  } else {
    ImGui::Separator();
    PS2Plus::Components::IconCheckmark(ImGui::GetFontSize(), COLOR_SUCCESS);
    ImGui::SameLine();
    ImGui::TextWrapped("Firmware update appears valid! Press \"Update\" below to begin.");
  }

  // Hack: Create a child window to move the action buttons to the bottom of the pane
  ImGui::BeginChild("ChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 8.f), false, ImGuiWindowFlags_NoNav);
  ImGui::EndChild();

  ImGui::Separator();

  if (ImGui::Button("Back")) {
    PS2Plus::Components::WizardPrevious();
  }
  ImGui::SameLine();

  ImGui::BeginDisabled(anyIssues && !acceptIssues);
  if (ImGui::Button("Update")) {
    state.current_controller()->update().status = kUpdateStatusPending;
    state.current_controller()->update().firmware = firmware_update;
    PS2Plus::Components::WizardNext();
  }
  ImGui::EndDisabled();

  if (anyIssues) {
    ImGui::SameLine();
    ImGui::Checkbox("Accept issues", &acceptIssues);
  }
}

void FirmwareWizardStep_Perform() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();

  float progress = 0.f;
  const char *label = NULL;
  PS2Plus::Components::StatusType status = PS2Plus::Components::kStatusLoading;

  switch (state.current_controller()->update().status) {
  case kUpdateStatusNone:
  case kUpdateStatusPending:
    label = "Update pending";
    status = PS2Plus::Components::kStatusLoading;
    break;

  case kUpdateStatusRebooting:
    label = "Rebooting controller";
    status = PS2Plus::Components::kStatusLoading;
    break;

  case kUpdateStatusUpdating:
  case kUpdateStatusUpdatePending:
    progress = (float)state.current_controller()->update().last_record_index / state.current_controller()->update().total_records;
    label = "Updating controller";
    status = PS2Plus::Components::kStatusLoading;
    break;

  case kUpdateStatusCompleted:
    label = "Firmware update complete!";
    status = PS2Plus::Components::kStatusSuccess;
    break;

  case kUpdateStatusFailed:
    label = "Error occurred during the update";
    status = PS2Plus::Components::kStatusError;
    break;
  }

  // Center align the status
  ImVec2 status_size = ImGui::CalcTextSize(label);
  ImVec2 total_status_size((ImGui::GetFontSize() * 0.85) + ImGui::GetStyle().ItemSpacing.x + status_size.x, status_size.y);
  ImGui::SetCursorPos((ImGui::GetWindowSize() - total_status_size) * 0.5f);
  PS2Plus::Components::StatusText(label, status);

  if (state.current_controller()->update().status == kUpdateStatusUpdating || state.current_controller()->update().status == kUpdateStatusUpdatePending) {
    float window_width = ImGui::GetWindowSize().x;
    float progress_padding = window_width * 0.1f;
    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(progress_padding, 0));
    ImGui::SetNextItemWidth(window_width - progress_padding * 2.f);
    PS2Plus::Components::LoadingProgress("##Progress", progress, "%0.f%%");
  }

  // Hack: Create a child window to move the action buttons to the bottom of the pane
  ImGui::BeginChild("ChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 8.f), false, ImGuiWindowFlags_NoNav);
  ImGui::EndChild();
  ImGui::Separator();

  ImGui::BeginDisabled(state.current_controller()->update().status != kUpdateStatusCompleted &&
                       state.current_controller()->update().status != kUpdateStatusFailed);
  if (ImGui::Button("Finish")) {
    path[0] = 0;
    state.current_controller()->update().status = kUpdateStatusNone;
    state.current_controller()->update().firmware = NULL;
    state.current_controller()->update().last_check_time = ImGui::GetTime();
    ;
    PS2Plus::Components::WizardGoToStep("Choose Update");
  }
  ImGui::EndDisabled();
}

void FirmwareSection() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();

  if (!state.current_controller() || !state.current_controller()->connected()) {
    NoPS2PlusConnectedMessage();
    return;
  }

  if (PS2Plus::Components::BeginWizard("UpdateFirmwareWizard", 3, &current_step)) {
    if (PS2Plus::Components::BeginWizardStepList()) {
      PS2Plus::Components::SetupWizardStep("Choose Update", current_step == 0 || current_step == 1);
      PS2Plus::Components::SetupWizardStep("Review Update", current_step == 1);
      PS2Plus::Components::SetupWizardStep("Perform Update", current_step == 2);
      PS2Plus::Components::EndWizardStepList();
    }

    if (PS2Plus::Components::BeginWizardStep("Choose Update")) {
      FirmwareWizardStep_Update();
      PS2Plus::Components::EndWizardStep();
    } else if (PS2Plus::Components::BeginWizardStep("Review Update")) {
      FirmwareWizardStep_Review();
      PS2Plus::Components::EndWizardStep();
    } else if (PS2Plus::Components::BeginWizardStep("Perform Update")) {
      FirmwareWizardStep_Perform();
      PS2Plus::Components::EndWizardStep();
    }

    PS2Plus::Components::EndWizard();
  }
}
} // namespace PS2Plus::App::Views