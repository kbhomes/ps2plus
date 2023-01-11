#include "controller-port-status.h"

#include "app/controller.h"
#include "app/state.h"
#include "ui/pad.h"

namespace PS2Plus::Components {
    enum ControllerPortType {
        kPS2PlusController,
        kNormalController,
        kDisconnected
    };

    constexpr ImColor GetControllerPortColor(ControllerPortType type) {
        switch (type) {
            case kPS2PlusController:    return IM_COL32(0x00, 0xFF, 0x00, 0xFF);
            case kNormalController:     return IM_COL32(0xFF, 0xA0, 0x00, 0xFF);
            case kDisconnected:         return ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
        };
    }

    const std::string GetControllerPortLabel(ControllerPortType type) {
        switch (type) {
            case kPS2PlusController:    return "PS2+ Controller";
            case kNormalController:     return "Normal Controller";
            case kDisconnected:         return "Disconnected";
        };
    }

    void ControllerPortStatus(int port) {
        PS2Plus::App::State& state = PS2Plus::App::GetState();
        PS2Plus::App::Controller *ps2plus = state.controllers().at(port);
        const PS2Plus::Gamepad::PadSummary& pad_summary = state.gamepad_summary();
        
        // Determine which type of controller is connected ont his port
        ControllerPortType port_type;
        if (ps2plus->connected())
            port_type = kPS2PlusController;
        else if (pad_summary.ports[port] == PS2Plus::Gamepad::PadPortReady)
            port_type = kNormalController;
        else
            port_type = kDisconnected;

        const ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        // Draw a custom icon
        const float icon_size = ImGui::GetFontSize();
        const ImVec2 icon_center = ImVec2(p.x + icon_size/2, p.y + icon_size/2);
        const ImColor icon_color = GetControllerPortColor(port_type);
        if (port_type == kPS2PlusController || port_type == kNormalController) {
            draw_list->AddCircleFilled(icon_center, icon_size/2, icon_color);
        } else {
            draw_list->AddCircle(icon_center, icon_size/2, icon_color);
        }
        ImGui::Dummy(ImVec2(icon_size, icon_size));
        ImGui::SameLine();
    
        // Draw the port label
        ImGui::TextColored(GetControllerPortColor(port_type), GetControllerPortLabel(port_type).c_str());
        ImGui::SameLine();
        ImGui::TextDisabled("Port %d", port + 1);
    }
}