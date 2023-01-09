#include "app.h"
#include "ui/fonts/playstation.h"
#include "components/tab-menu.h"
#include "components/controller-port-status.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include <functional>

bool dialog_displaying = false;

bool IsGamepadCaptured() {
    return !(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NavEnableGamepad);
}

void app_begin_dialog() {
    dialog_displaying = true;
}

void app_end_dialog() {
    dialog_displaying = false;
}


void app_no_controller_connected() {
    const char *labelDisconnected = "No PS2+ controller connected";
    ImVec2 labelSize = ImGui::CalcTextSize(labelDisconnected);
    ImGui::SetCursorPos((ImGui::GetWindowSize() - labelSize) * 0.5f);
    ImGui::Text(labelDisconnected);
}

void app_ps2plus_ports_display(ImGuiIO &io, configurator_state *state) {
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.f, 10.f));
    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(0.3, 0.3, 0.3, 0.5));
    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0.3, 0.3, 0.3, 0.5));

    int numControllers = sizeof(state->controllers) / sizeof(configurator_ps2plus_controller);
    if (ImGui::BeginTable("PS2+ Ports#Table", numControllers, ImGuiTableFlags_Borders)) {
        ImGui::TableNextRow();
        for (size_t i = 0; i < numControllers; i++) {
            ImGui::TableSetColumnIndex(i);
            PS2Plus::Components::ControllerPortStatus(state, i);
        }
        ImGui::EndTable();
    }

    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();
}

void app_display(ImGuiIO &io, configurator_state *state) {
    // Full screen "window" that can't be moved or resized
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10));
    ImGui::Begin("PS2+ Configurator", NULL, ImGuiWindowFlags_NoDecoration);
    {
        // PushStyleVar in previous lines was used to increase the title-bar height
        ImGui::PopStyleVar();

        // Show all the connected ports
        app_ps2plus_ports_display(io, state);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
        PS2Plus::Components::TabMenu("Sections", [&]{
            PS2Plus::Components::TabMenuItem("Information", std::bind(app_section_information, io, state));
            PS2Plus::Components::TabMenuItem("Configuration", std::bind(app_section_configuration, io, state));
            PS2Plus::Components::TabMenuItem("Firmware", std::bind(app_section_firmware, io, state));
            PS2Plus::Components::TabMenuItem("Test", std::bind(app_section_test, io, state));
            PS2Plus::Components::TabMenuItem("About", std::bind(app_section_about, io, state));
        });
        ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
    }
    ImGui::End();
}