#include "app.h"
#include "ui/fonts/playstation.h"
#include "ui/widgets/tab-menu/tab-menu.h"

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

void app_ps2plus_ports_display(ImGuiIO &io, configurator_state *state) {
    // Information section
    if (ImGui::BeginTable("PS2+ Ports#Table", 2)) {
        ImGui::TableNextRow();
        for (size_t i = 0; i < sizeof(state->controllers) / sizeof(configurator_ps2plus_controller); i++) {
            configurator_ps2plus_controller *controller = &state->controllers[i];

            ImGui::TableSetColumnIndex(i);
            
            // Draw a custom icon
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const float icon_size = ImGui::GetFontSize();
            const ImVec2 icon_center = ImVec2(p.x + icon_size/2, p.y + icon_size/2);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            if (controller->connected) {
                draw_list->AddCircleFilled(icon_center, icon_size/2, IM_COL32(0x00, 0xFF, 0x00, 0x80));
            } else {
                draw_list->AddCircle(icon_center, icon_size/2, IM_COL32(0xFF, 0xFF, 0xFF, 0x80));
            }
            ImGui::Dummy(ImVec2(icon_size, icon_size));
            ImGui::SameLine();
            
            ImGui::BeginGroup();
            {
                if (controller->connected) {
                    ImGui::TextColored(ImVec4(0, 1, 0, 1.0), "Connected to PS2+");
                    ImGui::SetWindowFontScale(0.75);
                    ImGui::Text("Port %d", i + 1);
                    ImGui::SetWindowFontScale(1.0);
                } else {
                    ImGui::TextDisabled("No PS2+ found");
                    ImGui::SetWindowFontScale(0.75);
                    ImGui::TextDisabled("Port %d", i + 1);
                    ImGui::SetWindowFontScale(1.0);
                }
            }
            ImGui::EndGroup();
        }
        ImGui::EndTable();
    }
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
        PS2Plus::UI::TabMenu("Sections", [&]{
            PS2Plus::UI::TabMenuItem("Information", std::bind(app_section_information, io, state));
            PS2Plus::UI::TabMenuItem("Test", std::bind(app_section_test, io, state));
            PS2Plus::UI::TabMenuItem("Configuration", std::bind(app_section_configuration, io, state));
            PS2Plus::UI::TabMenuItem("Firmware", std::bind(app_section_firmware, io, state));
            PS2Plus::UI::TabMenuItem("About", std::bind(app_section_about, io, state));
        });
        ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
    }
    ImGui::End();
}