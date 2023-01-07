#include "controller-port-status.h"

#include "../../ui/pad.h"

enum ControllerPortStatus_ {
    PS2PlusController,
    NormalController,
    Disconnected
};

const ImColor COLOR_PS2PLUS(IM_COL32(0x00, 0xFF, 0x00, 0xFF));
const ImColor COLOR_NORMAL(IM_COL32(0xFF, 0xA0, 0x00, 0xFF));
const ImColor COLOR_DISCONNECTED(IM_COL32(0xFF, 0xFF, 0xFF, 0xFF));

void PS2Plus::App::Views::ControllerPortStatus(configurator_state *state, int port) {
    configurator_ps2plus_controller *ps2plus = &state->controllers[port];
    const PS2Plus::Gamepad::PadSummary &padSummary = state->pad_summary;
    
    ControllerPortStatus_ portStatus;
    if (ps2plus->connected)
        portStatus = PS2PlusController;
    else if (padSummary.ports[port] == PS2Plus::Gamepad::PadPortReady)
        portStatus = NormalController;
    else
        portStatus = Disconnected;

    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float icon_size = ImGui::GetFontSize();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // Draw a custom icon
    const ImVec2 icon_center = ImVec2(p.x + icon_size/2, p.y + icon_size/2);
    if (portStatus == PS2PlusController) {
        draw_list->AddCircleFilled(icon_center, icon_size/2, COLOR_PS2PLUS);
    } else if (portStatus == NormalController) {
        draw_list->AddCircleFilled(icon_center, icon_size/2, COLOR_NORMAL);
    } else {
        draw_list->AddCircle(icon_center, icon_size/2, COLOR_DISCONNECTED);
    }
    ImGui::Dummy(ImVec2(icon_size, icon_size));
    ImGui::SameLine();
    
    ImGui::BeginGroup();
    {
        if (portStatus == PS2PlusController) {
            ImGui::TextColored(COLOR_PS2PLUS, "PS2+ Controller");
        } else if (portStatus == NormalController) {
            ImGui::TextColored(COLOR_NORMAL, "Normal Controller");
        } else if (portStatus == Disconnected) {
            ImGui::TextDisabled("No Controller");
        }

        ImGui::SameLine();
        ImGui::TextDisabled("Port %d", port + 1);
    }
    ImGui::EndGroup();
}