#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <dirent.h>
#include <errno.h>
#include <map>
#include <stdio.h>
#include <string.h>

#include "app.h"

void app_section_firmware(ImGuiIO &io, configurator_state *state) {
  // open Dialog Simple
  if (ImGui::Button("Open File Dialog")) {
    ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", ".*", "host:/",  -1, nullptr, 
      // ImGuiFileDialogFlags_NoDialog | 
      ImGuiFileDialogFlags_HideColumnType | 
      ImGuiFileDialogFlags_HideColumnDate | 
      ImGuiFileDialogFlags_DisableCreateDirectoryButton | 
      ImGuiFileDialogFlags_ReadOnlyFileNameField);
  }

  // display
  if (ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey")) {
    app_begin_dialog();
    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 40, io.DisplaySize.y - 40));

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
      // action if OK
      if (ImGuiFileDialog::Instance()->IsOk()) {
        std::map<std::string, std::string> files = ImGuiFileDialog::Instance()->GetSelection();
        for (auto it = files.begin(); it != files.end(); it++)
        {
          printf("filePathName: %s\n", it->first.c_str());
          printf("filePath: %s\n", it->second.c_str());
        }
      }

      // close
      ImGuiFileDialog::Instance()->Close();
      app_end_dialog();
    }
  }
}