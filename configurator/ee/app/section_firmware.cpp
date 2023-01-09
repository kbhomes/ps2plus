#include "app.h"

#include <cstring>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "components/file-dialog.h"
#include "components/icons.h"
#include "components/loading-progress.h"
#include "components/status.h"
#include "components/wizard.h"
#include <util/firmware-update.h>
#include <util/versions.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#define MAX_PATH 255

const ImU32 COLOR_SUCCESS = IM_COL32(0x00, 0xFF, 0x00, 0x80);
const ImU32 COLOR_ERROR = IM_COL32(0xFF, 0x00, 0x00, 0x80);
const ImU32 COLOR_WARNING = IM_COL32(0xFF, 0xFF, 0x00, 0x80);

std::vector<std::string> deviceList = { "host:", "mc0:", "mc1:", "mass:", "mass0:", "mass1:" };
char path[MAX_PATH];
int currentStep = 0;
FirmwareUpdate *firmwareUpdate;

enum FirmwareUpdateType {
  NoChange, Upgrade, Downgrade
};

void wizard_choose_update(ImGuiIO &io, configurator_state *state) {
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
    if (ImGui::Button("Browse")) {
      ImGui::OpenPopup("Choose firmware update file");
    }
    // Display the file chooser popup
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 20, io.DisplaySize.y - 20));
    if (ImGui::BeginPopupModal("Choose firmware update file")) {
      app_begin_dialog();

      if (PS2Plus::Components::FileDialog("FileDialog-FirmwareUpdate", NULL, deviceList, path)) {
        printf("Got path: %s\n", path);
        ImGui::CloseCurrentPopup();
        app_end_dialog();
      }

      ImGui::EndPopup();
    }
    
    ImGui::EndTable();
  }

  ImGui::Separator();

  ImGui::TextWrapped("Manually update PS2+ firmware:");
  ImGui::Bullet(); ImGui::TextWrapped("Download the latest PS2+ firmware update file from <TBD>");
  ImGui::Indent();
    ImGui::Bullet(); ImGui::TextWrapped("Microcontroller: %s", state->current_controller->versions.microcontroller);
    ImGui::Bullet(); ImGui::TextWrapped("Update Only (not Bootloader or Combined)");
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
    app_begin_dialog();
    for (auto testFilename : FirmwareUpdate::GetTestFilenames()) {
      if (ImGui::Selectable(testFilename.c_str())) {
        // Read the test firmware
        firmwareUpdate = new FirmwareUpdate(testFilename);
        PS2Plus::Components::WizardNext();
        app_end_dialog();
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
    firmwareUpdate = new FirmwareUpdate(path);
    PS2Plus::Components::WizardNext();
  }
  
  ImGui::EndDisabled();
}

void wizard_review_update(ImGuiIO &io, configurator_state *state) {
  uint64_t versionFirmwareCurrent = state->current_controller->versions.firmware;
  uint64_t versionFirmwareUpdate = firmwareUpdate->GetFirmwareVersion();
  FirmwareUpdateType versionFirmwareUpdateType = 
      (versionFirmwareUpdate > versionFirmwareCurrent) ? Upgrade :
      (versionFirmwareUpdate < versionFirmwareCurrent) ? Downgrade :
      NoChange;
      
  char *versionMicrocontrollerCurrent = state->current_controller->versions.microcontroller;
  std::string versionMicrocontrollerUpdate = firmwareUpdate->GetMicrocontrollerVersion();
  bool versionMicrocontrollerMismatch = std::strcmp(versionMicrocontrollerUpdate.c_str(), versionMicrocontrollerCurrent) != 0;

  uint8_t versionConfigurationCurrent = state->current_controller->versions.configuration;
  uint8_t versionConfigurationUpdate = firmwareUpdate->GetConfigurationVersion();
  bool versionConfigurationMismatch = (versionConfigurationCurrent != versionConfigurationUpdate);

  static bool acceptIssues = false;
  bool anyIssues = (versionFirmwareUpdateType != Upgrade) || versionMicrocontrollerMismatch || versionConfigurationMismatch;

  if (ImGui::BeginTable("FirmwareUpdateVersions", 3, ImGuiTableFlags_Borders)) {
    ImGui::TableSetupColumn("Version", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Current", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Update", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    ImGui::TableNextColumn(); 
    ImGui::Text("Firmware");
    ImGui::TableNextColumn(); 
    ImGui::Text("%s", format_version(state->current_controller->versions.firmware).c_str());
    ImGui::TableNextColumn(); 
    if (versionFirmwareUpdateType == Upgrade)
      PS2Plus::Components::IconArrow(ImGui::GetFontSize(), COLOR_SUCCESS, ImGuiDir_Up);
    else if (versionFirmwareUpdateType == Downgrade)
      PS2Plus::Components::IconArrow(ImGui::GetFontSize(), COLOR_ERROR, ImGuiDir_Down);
    else if (versionFirmwareUpdateType == NoChange)
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
    ImGui::SameLine();
    ImGui::Text("%s", format_version(firmwareUpdate->GetFirmwareVersion()).c_str());

    ImGui::TableNextRow(); 
    ImGui::TableNextColumn(); ImGui::Text("Microcontroller");
    ImGui::TableNextColumn(); ImGui::Text("%s", state->current_controller->versions.microcontroller);
    ImGui::TableNextColumn(); 
    if (versionMicrocontrollerMismatch)
      PS2Plus::Components::IconError(ImGui::GetFontSize(), COLOR_ERROR);
    else
      PS2Plus::Components::IconCheckmark(ImGui::GetFontSize(), COLOR_SUCCESS);
    ImGui::SameLine();
    ImGui::Text("%s", firmwareUpdate->GetMicrocontrollerVersion().c_str());

    ImGui::TableNextRow();
    ImGui::TableNextColumn(); ImGui::Text("Configuration");
    ImGui::TableNextColumn(); ImGui::Text("%d", state->current_controller->versions.configuration);
    ImGui::TableNextColumn(); 
    if (versionConfigurationMismatch)
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
    else
      PS2Plus::Components::IconCheckmark(ImGui::GetFontSize(), COLOR_SUCCESS);
    ImGui::SameLine();
    ImGui::Text("%d", firmwareUpdate->GetConfigurationVersion());

    ImGui::EndTable();
  }
  
  if (anyIssues) {
    ImGui::Separator();
    ImGui::Text("Issues:");

    if (versionFirmwareUpdateType == Downgrade) {
      ImGui::Bullet();
      PS2Plus::Components::IconArrow(ImGui::GetFontSize(), COLOR_ERROR, ImGuiDir_Down);
      ImGui::SameLine(); 
      ImGui::TextWrapped("Firmware will downgrade the PS2+");
    } else if (versionFirmwareUpdateType == NoChange) {
      ImGui::Bullet();
      PS2Plus::Components::IconWarning(ImGui::GetFontSize(), COLOR_WARNING);
      ImGui::SameLine(); 
      ImGui::TextWrapped("Firmware may already be on the PS2+");
    }

    if (versionMicrocontrollerMismatch) {
      ImGui::Bullet();
      PS2Plus::Components::IconError(ImGui::GetFontSize(), COLOR_ERROR);
      ImGui::SameLine(); 
      ImGui::TextWrapped("Firmware is not for this microcontroller and may damage the PS2+");
    } 

    if (versionConfigurationMismatch) {
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
    state->current_controller->update.status = StatusPending;
    state->current_controller->update.firmware = firmwareUpdate;
    PS2Plus::Components::WizardNext();
  }
  ImGui::EndDisabled();

  if (anyIssues) {
    ImGui::SameLine();
    ImGui::Checkbox("Accept issues", &acceptIssues);
  }
}

void wizard_perform_update(ImGuiIO &io, configurator_state *state) {
  float progress = 0.f;
  const char *label = NULL;
  PS2Plus::Components::StatusType status = PS2Plus::Components::kStatusLoading;

  switch (state->current_controller->update.status) {
    case StatusNone:
    case StatusPending:
      label = "Update pending";
      status = PS2Plus::Components::kStatusLoading;
      break;

    case StatusRebooting:
      label = "Rebooting controller";
      status = PS2Plus::Components::kStatusLoading;
      break;

    case StatusUpdating:
    case StatusUpdatePending:
      progress = (float)state->current_controller->update.last_record_index / state->current_controller->update.total_records;
      label = "Updating controller";
      status = PS2Plus::Components::kStatusLoading;
      break;

    case StatusCompleted:
      label = "Firmware update complete!";
      status = PS2Plus::Components::kStatusSuccess;
      break;

    case StatusFailed:
      label = "Error occurred during the update";
      status = PS2Plus::Components::kStatusError;
      break;
  }

  // Center align the status
  ImVec2 statusSize = ImGui::CalcTextSize(label);
  ImVec2 totalStatusSize((ImGui::GetFontSize() * 0.85) + ImGui::GetStyle().ItemSpacing.x + statusSize.x, statusSize.y);
  ImGui::SetCursorPos((ImGui::GetWindowSize() - totalStatusSize) * 0.5f);
  PS2Plus::Components::StatusText(label, status);

  if (state->current_controller->update.status == StatusUpdating || state->current_controller->update.status == StatusUpdatePending) {
    float windowWidth = ImGui::GetWindowSize().x;
    float progressPadding = windowWidth * 0.1f;
    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(progressPadding, 0));
    ImGui::SetNextItemWidth(windowWidth - progressPadding*2.f);
    PS2Plus::Components::LoadingProgress("##Progress", progress, "%0.f%%");
  }

  // Hack: Create a child window to move the action buttons to the bottom of the pane
  ImGui::BeginChild("ChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 8.f), false, ImGuiWindowFlags_NoNav);
  ImGui::EndChild(); 
  ImGui::Separator();

  ImGui::BeginDisabled(state->current_controller->update.status != StatusCompleted && state->current_controller->update.status != StatusFailed);
  if (ImGui::Button("Finish")) {
    path[0] = 0;
    state->current_controller->update.status = StatusNone;
    state->current_controller->update.firmware = NULL;
    state->current_controller->update.last_check_time = ImGui::GetTime();;
    PS2Plus::Components::WizardGoToStep("Choose Update");
  }
  ImGui::EndDisabled();
}

void app_section_firmware(ImGuiIO &io, configurator_state *state) {
  if (!state->current_controller || !state->current_controller->connected) {
    app_no_controller_connected();
    return;
  }
  
  if (PS2Plus::Components::BeginWizard("UpdateFirmwareWizard", 3, &currentStep)) {
    if (PS2Plus::Components::BeginWizardStepList()) {
      PS2Plus::Components::SetupWizardStep("Choose Update", currentStep == 0 || currentStep == 1);
      PS2Plus::Components::SetupWizardStep("Review Update", currentStep == 1);
      PS2Plus::Components::SetupWizardStep("Perform Update", currentStep == 2);
      PS2Plus::Components::EndWizardStepList();
    }

    if (PS2Plus::Components::BeginWizardStep("Choose Update")) {
      wizard_choose_update(io, state);
      PS2Plus::Components::EndWizardStep();
    } else if (PS2Plus::Components::BeginWizardStep("Review Update")) {
      wizard_review_update(io, state);
      PS2Plus::Components::EndWizardStep();
    } else if (PS2Plus::Components::BeginWizardStep("Perform Update")) {
      wizard_perform_update(io, state);
      PS2Plus::Components::EndWizardStep();
    }

    PS2Plus::Components::EndWizard();
  }
}