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
  if (ImGui::Button("Choose file")) {
    ImGui::OpenPopup("Choose firmware update file");
  }
  ImGui::SameLine();
  ImGui::TextWrapped("%s", path);

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

  bool hasPath = path[0] != 0;
  ImGui::BeginDisabled(!hasPath);
  if (ImGui::Button("Next")) {
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
    ImGui::Widgets::WizardGoToStep("Choose Update");
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