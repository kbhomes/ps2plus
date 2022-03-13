#include "app.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <ui/widgets/file-dialog/file-dialog.h>

#define MAX_PATH 255

std::vector<std::string> deviceList = { "host:", "mc0:", "mc1:", "mass:" };

void app_section_firmware(ImGuiIO &io, configurator_state *state) {
  static char path[MAX_PATH];

  if (ImGui::Widgets::FileDialog("FileDialog-FirmwareUpdate", "host:/", deviceList, path)) {
    printf("Got path: %s\n", path);
  }
}