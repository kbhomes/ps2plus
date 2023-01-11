#include "app.h"

#include "components/help-marker.h"

namespace PS2Plus::App::Views {
void InformationSection() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();

  if (!state.current_controller() || !state.current_controller()->connected()) {
    NoPS2PlusConnectedMessage();
    return;
  }

  ImGui::BeginChild("InformationChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NavFlattened);

  // Version data
  if (ImGui::BeginTable("PS2+ Version Number", 2, ImGuiTableFlags_Borders)) {
    ImGui::TableSetupColumn("Version Number", ImGuiTableColumnFlags_WidthFixed);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Firmware");
    ImGui::SameLine();
    PS2Plus::Components::HelpMarker("Version of the PS2+'s firmware.");
    ImGui::TableNextColumn();
    ImGui::TextDisabled("%s", state.current_controller()->versions().FirmwareString().c_str());

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Microcontroller");
    ImGui::SameLine();
    PS2Plus::Components::HelpMarker("Name of the the PS2+'s microcontroller. "
                                    "Firmware updates must be for this same microcontroller.");
    ImGui::TableNextColumn();
    ImGui::TextDisabled("%s", state.current_controller()->versions().microcontroller);

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Configuration");
    ImGui::SameLine();
    PS2Plus::Components::HelpMarker("Version of the PS2+'s internal configuration format. "
                                    "Firmware updates that change the configuration version will "
                                    "reset the PS2+'s configuration to avoid "
                                    "incompatibilities.");
    ImGui::TableNextColumn();
    ImGui::TextDisabled("%d", state.current_controller()->versions().configuration);

    ImGui::EndTable();
  }

  ImGui::Separator();

  ImGui::EndChild();
}
} // namespace PS2Plus::Apps::Views