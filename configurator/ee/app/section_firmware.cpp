#include "app.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <ui/widgets/file-dialog/file-dialog.h>

#define MAX_PATH 255

std::vector<std::string> deviceList = { "host:", "mc0:", "mc1:", "mass:" };

void app_section_firmware(ImGuiIO &io, configurator_state *state) {
  static char path[MAX_PATH];

  if (ImGui::Button("Choose file")) {
    ImGui::OpenPopup("Choose firmware update file");
  }
  ImGui::SameLine();
  ImGui::TextWrapped("%s", path);

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
}