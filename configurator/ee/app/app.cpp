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
    // // Whether the user is selecting a section in the left pane
    // static bool is_selecting_section = false;

    // // Actively displayed section content
    // static int selected_content = 0;

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
            if (ImGui::TabItemButton(ICON_PLAYSTATION_L1_BUTTON_ALT, ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip)) {
                active_tab = (active_tab > 0) ? active_tab - 1 : tab_count - 1;
            }

            if (ImGui::TabItemButton(ICON_PLAYSTATION_R1_BUTTON_ALT, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
                active_tab = (active_tab < tab_count - 1) ? active_tab + 1 : 0;
            }
            ImGui::PopStyleColor(/* ImGuiCol_Tab */);

            if (state->pad_status.IsButtonPressed(PAD_L1)) {
                active_tab = (active_tab > 0) ? active_tab - 1 : tab_count - 1;
            }

            if (state->pad_status.IsButtonPressed(PAD_R1)) {
                active_tab = (active_tab < tab_count - 1) ? active_tab + 1 : 0;
            }

            if (ImGui::BeginTabItem("Information", NULL, active_tab == 0 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                ImGui::BeginChild("InformationChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NavFlattened);
                app_section_information(io, state);
                ImGui::EndChild();
                ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Tests", NULL, active_tab == 1 ? ImGuiTabItemFlags_SetSelected : 0)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
                ImGui::ShowStyleEditor();
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

            ImGui::EndTabBar();
        }
        ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);



        // if (is_selecting_section && !dialog_displaying) {
        //     // Only allow the left pane to have focus while the user selects
        //     ImGui::SetNextWindowFocus();
        // }
        // ImGui::BeginChild("List", ImVec2(150, 0), true);
        // {
        //     if (ImGui::Selectable("Information", !is_selecting_section && selected_content == 0, 0, ImVec2(0, 25))) {
        //         selected_content = 0;
        //         is_selecting_section = false;
        //     }
        //     if (ImGui::Selectable("Configuration", !is_selecting_section && selected_content == 1, 0, ImVec2(0, 25))) {
        //         selected_content = 1;
        //         is_selecting_section = false;
        //     }
        //     if (ImGui::Selectable("Firmware", !is_selecting_section && selected_content == 2, 0, ImVec2(0, 25))) {
        //         selected_content = 2;
        //         is_selecting_section = false;
        //     }
        //     if (ImGui::Selectable("About", !is_selecting_section && selected_content == 3, 0, ImVec2(0, 25))) {
        //         selected_content = 3;
        //         is_selecting_section = false;
        //     }
        // }
        // ImGui::EndChild();

        // ImGui::SameLine();
        // ImGui::BeginGroup();
        // {
        //     // Content pane will retain focus if it is selected
        //     if (selected_content >= 0 && !is_selecting_section && !dialog_displaying) {
        //         ImGui::SetNextWindowFocus();
        //     }

        //     app_ps2plus_ports_display(io, state);

        //     // Content pane has full height, less the height of a single frame which will be used to
        //     // display contextual button controls
        //     ImGui::BeginDisabled(is_selecting_section);
        //     ImGui::BeginChild("ChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
        //     {
        //         if (selected_content == 0) {
        //             app_section_information(io, state);
        //         }

        //         if (selected_content == 1) {
        //             app_section_configuration(io, state);
        //         }

        //         if (selected_content == 2) {
        //             app_section_firmware(io, state);
        //         }
                
        //         if (selected_content == 3) {
        //             app_section_about(io, state);
        //         }

        //         // Allow the user to change sections using the triangle button
        //         if (!IsGamepadCaptured() && !dialog_displaying && (state->pad_status.IsButtonPressed(PAD_TRIANGLE))) {
        //             is_selecting_section = !is_selecting_section;
        //         }

        //         // Dim this content section window if the user is selecting a different section
        //         if (is_selecting_section) {
        //             PS2Plus::UI::WindowOverlay(0.6f);
        //         }
        //     }
        //     ImGui::EndChild();
        //     ImGui::EndDisabled();

        //     // Bottom row with control information
        //     {
        //         PS2Plus::UI::GamePadIcon(PS2Plus::UI::WidgetGamePadIconType_Triangle);
        //         ImGui::SameLine();
        //         ImGui::Text("Change Section");
        //         ImGui::SameLine();
        //     }
        // }
        // ImGui::EndGroup();
    }
    ImGui::End();
}