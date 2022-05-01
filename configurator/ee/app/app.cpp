#include "app.h"
#include "ui/fonts/playstation.h"

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
    static int active_tab = 0;
    static int tab_count = 5;

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
        if (ImGui::BeginTabBar("Sections")) {
            // Control buttons on the ends have transparent backgrounds
            ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            ImGui::BeginDisabled(!PS2Plus::Graphics::IsGamepadNavActive());
            if (ImGui::TabItemButton(ICON_PLAYSTATION_L1_BUTTON_ALT, ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip)) {
                active_tab = (active_tab > 0) ? active_tab - 1 : tab_count - 1;
            }

            if (ImGui::TabItemButton(ICON_PLAYSTATION_R1_BUTTON_ALT, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
                active_tab = (active_tab < tab_count - 1) ? active_tab + 1 : 0;
            }
            ImGui::EndDisabled();
            ImGui::PopStyleColor(/* ImGuiCol_Text */);
            ImGui::PopStyleColor(/* ImGuiCol_Tab */);

            // Indicate disabled tabs
            ImGui::PushStyleColor(ImGuiCol_Tab, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_Tab) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabActive, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabActive) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabHovered, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabHovered) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabUnfocused, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocused) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive) : ImVec4(0, 0, 0, 0));

            if (PS2Plus::Graphics::IsGamepadNavActive()) {
                if (state->pad_status.IsButtonPressed(PAD_L1)) {
                    active_tab = (active_tab > 0) ? active_tab - 1 : tab_count - 1;
                }

                if (state->pad_status.IsButtonPressed(PAD_R1)) {
                    active_tab = (active_tab < tab_count - 1) ? active_tab + 1 : 0;
                }
            }

            if (ImGui::BeginTabItem("Information", NULL, active_tab == 0 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                ImGui::BeginChild("InformationChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NavFlattened);
                app_section_information(io, state);
                ImGui::EndChild();
                ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Test", NULL, active_tab == 1 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                app_section_test(io, state);
                ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Configuration", NULL, active_tab == 2 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                app_section_configuration(io, state);
                ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Firmware", NULL, active_tab == 3 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                app_section_firmware(io, state);
                ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("About", NULL, active_tab == 4 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                app_section_about(io, state);
                ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
                ImGui::EndTabItem();
            }

            ImGui::PopStyleColor(5 /* ImGuiCol_Tab/TabActive/TabHovered/TabUnfocused/TabUnfocusedActive */);

            ImGui::EndTabBar();
        }
        ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
    }
    ImGui::End();
}