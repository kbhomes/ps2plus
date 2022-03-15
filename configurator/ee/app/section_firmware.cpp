#include "app.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <ui/widgets/file-dialog/file-dialog.h>
#include <ui/widgets/wizard/wizard.h>

#define MAX_PATH 255

std::vector<std::string> deviceList = { "host:", "mc0:", "mc1:", "mass:" };
char path[MAX_PATH];
int currentStep = 0;

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

      if (ImGui::Widgets::FileDialog("FileDialog-FirmwareUpdate", "host:/", deviceList, path)) {
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
  ImGui::Bullet(); ImGui::TextWrapped("Save the update file to a USB flash drive and insert it into your PS2");
  ImGui::Bullet(); ImGui::TextWrapped("Press \"Browse\" above and choose the update file");
  ImGui::Bullet(); ImGui::TextWrapped("Press \"Continue\" below to review the selected update");

  ImGui::Separator();

  ImVec2 actionButtonSize = ImVec2(100, 0);
  ImGui::Dummy(ImVec2());
  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - actionButtonSize.x);
  bool hasPath = path[0] != 0;
  ImGui::BeginDisabled(!hasPath);
  if (ImGui::Button("Continue", actionButtonSize)) {
    ImGui::Widgets::WizardNext();
  }
  ImGui::EndDisabled();
}

void wizard_review_update(ImGuiIO &io, configurator_state *state) {
  ImGui::Text("Step 2!");

  if (ImGui::Button("Previous")) {
    ImGui::Widgets::WizardPrevious();
  }
  ImGui::SameLine();
  if (ImGui::Button("Next")) {
    ImGui::Widgets::WizardNext();
  }
}

void wizard_perform_update(ImGuiIO &io, configurator_state *state) {
  ImGui::Text("Step 3!");

  if (ImGui::Button("Finish")) {
    ImGui::Widgets::WizardGoToStep("1. Choose Update");
  }
}

void app_section_firmware(ImGuiIO &io, configurator_state *state) {

  if (ImGui::Widgets::BeginWizard("UpdateFirmwareWizard", 3, &currentStep)) {
    if (ImGui::Widgets::BeginWizardStepList()) {
      ImGui::Widgets::SetupWizardStep("Choose Update", currentStep == 0 || currentStep == 1);
      ImGui::Widgets::SetupWizardStep("Review Update", currentStep == 1);
      ImGui::Widgets::SetupWizardStep("Perform Update", currentStep == 2);
      ImGui::Widgets::EndWizardStepList();
    }

    if (ImGui::Widgets::BeginWizardStep("Choose Update")) {
      wizard_choose_update(io, state);
      ImGui::Widgets::EndWizardStep();
    } else if (ImGui::Widgets::BeginWizardStep("Review Update")) {
      wizard_review_update(io, state);
      ImGui::Widgets::EndWizardStep();
    } else if (ImGui::Widgets::BeginWizardStep("Perform Update")) {
      wizard_perform_update(io, state);
      ImGui::Widgets::EndWizardStep();
    }

    ImGui::Widgets::EndWizard();
  }
}