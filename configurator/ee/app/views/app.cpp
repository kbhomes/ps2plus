#include "app.h"
#include "components/controller-port-status.h"
#include "components/tab-menu.h"

#include <functional>

namespace PS2Plus::App::Views {

// Forward declarations of each tab renderer
void InformationSection();
void ConfigurationSection();
void FirmwareSection();
void TestSection();
void AboutSection();

void NoPS2PlusConnectedMessage() {
  const char *labelDisconnected = "No PS2+ controller connected";
  ImVec2 labelSize = ImGui::CalcTextSize(labelDisconnected);
  ImGui::SetCursorPos((ImGui::GetWindowSize() - labelSize) * 0.5f);
  ImGui::Text(labelDisconnected);
}

void ControllerPortsDisplay() {
  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.f, 10.f));
  ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(0.3, 0.3, 0.3, 0.5));
  ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0.3, 0.3, 0.3, 0.5));

  size_t num_controllers = PS2Plus::App::GetState().controllers().size();
  if (ImGui::BeginTable("#ControllerPorts", num_controllers, ImGuiTableFlags_Borders)) {
    ImGui::TableNextRow();
    for (size_t i = 0; i < num_controllers; i++) {
      ImGui::TableSetColumnIndex(i);
      PS2Plus::Components::ControllerPortStatus(i);
    }
    ImGui::EndTable();
  }

  ImGui::PopStyleColor(2);
  ImGui::PopStyleVar();
}

void BeginCaptureGamepad() {
  ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
  ImGui::GetIO().MouseDrawCursor = false;
}

void EndCaptureGamepad() { 
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
}

void MainView() {
  // Full screen "window" that can't be moved or resized
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("PS2+ Configurator", NULL, ImGuiWindowFlags_NoDecoration);
  {
    // Show all the connected ports
    ControllerPortsDisplay();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
    PS2Plus::Components::TabMenu("Sections", [&] {
      PS2Plus::Components::TabMenuItem("Information", InformationSection);
      PS2Plus::Components::TabMenuItem("Configuration", ConfigurationSection);
      PS2Plus::Components::TabMenuItem("Firmware", FirmwareSection);
      PS2Plus::Components::TabMenuItem("Test", TestSection);
      PS2Plus::Components::TabMenuItem("About", AboutSection);
    });
    ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
  }
  ImGui::End();
}
} // namespace PS2Plus::App::Views