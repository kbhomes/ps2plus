#include "app.h"

void BeginCaptureGamepad() {
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    ImGui::GetIO().MouseDrawCursor = false;
}

void EndCaptureGamepad() {
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

void app_configuration_button_remapping(ImGuiIO &io, configurator_state *state) {
    static bool enable_button_remapping = state->current_controller->configuration.enable_button_remapping.boolean;
    
    ImGui::Checkbox("##enable_button_remapping", &enable_button_remapping); 
    ImGui::SameLine(); ImGui::TextWrapped("Enable remapping of digital controller buttons");
}

void app_configuration_buttons(ImGuiIO &io, configurator_state *state) {
    if (ImGui::TreeNodeEx("Buttons", ImGuiTreeNodeFlags_DefaultOpen)) {
        app_configuration_button_remapping(io, state);

        ImGui::TreePop();
        ImGui::Separator();
    }
}

void app_configuration_joysticks_deadzones(ImGuiIO &io, configurator_state *state) {
    static uint8_t joystick_deadzone_left = state->current_controller->configuration.joystick_deadzone_left.uint8;
    static uint8_t joystick_deadzone_right = state->current_controller->configuration.joystick_deadzone_right.uint8;
    static uint8_t value_min = 0, value_max = 127;

    if (ImGui::TreeNodeEx("Deadzones##Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::BeginGroup();
        {
            const float joystick_size = 90.f;
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const ImVec2 center = ImVec2(p.x + joystick_size/2, p.y + joystick_size/2);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            draw_controller_joystick(
                draw_list, 
                center, 
                joystick_size / 2, false, state->pad_status.pad.ljoy_h, state->pad_status.pad.ljoy_v);

            if (joystick_deadzone_left) {
                draw_list->AddCircleFilled(center, joystick_size/2 * (joystick_deadzone_left / 128.f), IM_COL32(0xC0, 0x33, 0x33, 0x33));
            }

            ImGui::Dummy(ImVec2(joystick_size, joystick_size));
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            const float joystick_size = 90.f;
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const ImVec2 center = ImVec2(p.x + joystick_size/2, p.y + joystick_size/2);
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            draw_controller_joystick(
                draw_list, 
                center, 
                joystick_size / 2, false, state->pad_status.pad.rjoy_h, state->pad_status.pad.rjoy_v);

            if (joystick_deadzone_right) {
                draw_list->AddCircleFilled(center, joystick_size/2 * (joystick_deadzone_right / 128.f), IM_COL32(0xC0, 0x33, 0x33, 0x33));
            }

            ImGui::Dummy(ImVec2(joystick_size, joystick_size));
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            ImGui::PushItemWidth(50.f);
            ImGui::DragScalar("Left", ImGuiDataType_U8, &joystick_deadzone_left, 1.0, &value_min, &value_max);
            ImGui::DragScalar("Right", ImGuiDataType_U8, &joystick_deadzone_right, 1.0, &value_min, &value_max);
            ImGui::PopItemWidth();
        }
        ImGui::EndGroup();

        ImGui::TreePop();
        ImGui::Separator();
    }
}

void app_configuration_joysticks_digital_mode(ImGuiIO &io, configurator_state *state) {
    static uint8_t joystick_digital_threshold_left = state->current_controller->configuration.joystick_digital_threshold_left.uint8;
    static uint8_t joystick_digital_threshold_right = state->current_controller->configuration.joystick_digital_threshold_right.uint8;

    static int joystick_digital_enable_flags = (
        (state->current_controller->configuration.joystick_digital_enable_left.boolean ? 0b01 : 0b00) | 
        (state->current_controller->configuration.joystick_digital_enable_right.boolean ? 0b10 : 0b00));
    static uint8_t threshold_min = 0, threshold_max = 0x7F;

    bool joystick_digital_enable_left = joystick_digital_enable_flags & 0b01;
    bool joystick_digital_enable_right = joystick_digital_enable_flags & 0b10;

    if (ImGui::TreeNodeEx("Digital Mode##Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGui::CheckboxFlags("Enable using the joysticks in digital mode", &joystick_digital_enable_flags, 0b11);
        ImGui::Indent(); 
        {
            ImGui::CheckboxFlags("##Joysticks/joystick_digital_enable_left", &joystick_digital_enable_flags, 0b01);
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
                ImGui::TextWrapped("Left joystick simulates the D-pad");
                ImGui::BeginDisabled(!joystick_digital_enable_left);
                {
                    ImGui::Text("Threshold: ");
                    ImGui::SameLine();
                    ImGui::PushItemWidth(50.f);
                    ImGui::DragScalar("##Joysticks/joystick_digital_threshold_left", ImGuiDataType_U8, &joystick_digital_threshold_left, 1.0, &threshold_min, &threshold_max);
                    ImGui::PopItemWidth();
                }
                ImGui::EndDisabled();
            }
            ImGui::EndGroup();
            
            ImGui::CheckboxFlags("##Joysticks/joystick_digital_enable_right", &joystick_digital_enable_flags, 0b10);
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
                ImGui::TextWrapped("Right joystick simulates the face buttons");
                ImGui::BeginDisabled(!joystick_digital_enable_right);
                {
                    ImGui::Text("Threshold: ");
                    ImGui::SameLine();
                    ImGui::PushItemWidth(50.f);
                    ImGui::DragScalar("##Joysticks/joystick_digital_threshold_right", ImGuiDataType_U8, &joystick_digital_threshold_right, 1.0, &threshold_min, &threshold_max);
                    ImGui::PopItemWidth();
                }
                ImGui::EndDisabled();
            }
            ImGui::EndGroup();
        }
        ImGui::Unindent();
        
        ImGui::TreePop();
        ImGui::Separator();
    }
}

void app_configuration_joysticks_axis_range_remapping(ImGuiIO &io, configurator_state *state) {
    static bool enable_joystick_axis_range_remapping = state->current_controller->configuration.enable_joystick_axis_range_remapping.boolean;
    static uint8_t joystick_axis_range_remappings[12] = { 0, 127, 255, 0, 127, 255, 0, 127, 255, 0, 127, 255 };

    if (ImGui::TreeNodeEx("Axis Range Remapping##Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {
        static bool is_calibrating = false;

        ImGui::Checkbox("##enable_button_remapping", &enable_joystick_axis_range_remapping); 
        ImGui::SameLine(); ImGui::TextWrapped("Enable joystick axis range remapping");

        ImGui::BeginDisabled(!enable_joystick_axis_range_remapping);
        ImGui::BeginGroup();
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const float joystick_size = 90.f;
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            draw_controller_joystick(
                draw_list, 
                ImVec2(p.x + joystick_size/2, p.y + joystick_size/2), 
                joystick_size / 2, is_calibrating, joystick_axis_range_remappings[JSAxisRange_LeftXCenter], joystick_axis_range_remappings[JSAxisRange_LeftYCenter]);

            draw_list->AddRectFilled(
                ImVec2(p.x + joystick_size/2 - joystick_size/2*(1 - (float)joystick_axis_range_remappings[JSAxisRange_LeftXMin]/128), p.y + joystick_size/2 - joystick_size/2*(1 - (float)joystick_axis_range_remappings[JSAxisRange_LeftYMin]/128)),
                ImVec2(p.x + joystick_size/2 + joystick_size/2*((float)joystick_axis_range_remappings[JSAxisRange_LeftXMax]/128 - 1), p.y + joystick_size/2 + joystick_size/2*((float)joystick_axis_range_remappings[JSAxisRange_LeftYMax]/128 - 1)),
                IM_COL32(0x55, 0x55, 0xAA, 0x60), 5.0f);

            if (is_calibrating) {
                draw_list->AddCircleFilled(
                    ImVec2(p.x + joystick_size*(state->pad_status.pad.ljoy_h/255.f), p.y + joystick_size*(state->pad_status.pad.ljoy_v/255.f)),
                    2.0f, IM_COL32(0x55, 0x55, 0xAA, 0xC0));
            }

            ImGui::Dummy(ImVec2(joystick_size, joystick_size));
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            const float joystick_size = 90.f;
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            draw_controller_joystick(
                draw_list, 
                ImVec2(p.x + joystick_size/2, p.y + joystick_size/2), 
                joystick_size / 2, is_calibrating, joystick_axis_range_remappings[JSAxisRange_RightXCenter], joystick_axis_range_remappings[JSAxisRange_RightYCenter]);

            draw_list->AddRectFilled(
                ImVec2(p.x + joystick_size/2 - joystick_size/2*(1 - (float)joystick_axis_range_remappings[JSAxisRange_RightXMin]/128), p.y + joystick_size/2 - joystick_size/2*(1 - (float)joystick_axis_range_remappings[JSAxisRange_RightYMin]/128)),
                ImVec2(p.x + joystick_size/2 + joystick_size/2*((float)joystick_axis_range_remappings[JSAxisRange_RightXMax]/128 - 1), p.y + joystick_size/2 + joystick_size/2*((float)joystick_axis_range_remappings[JSAxisRange_RightYMax]/128 - 1)),
                IM_COL32(0x55, 0x55, 0xAA, 0x60), 5.0f);

            if (is_calibrating) {
                draw_list->AddCircleFilled(
                    ImVec2(p.x + joystick_size*(state->pad_status.pad.rjoy_h/255.f), p.y + joystick_size*(state->pad_status.pad.rjoy_v/255.f)),
                    2.0f, IM_COL32(0x55, 0x55, 0xAA, 0xC0));
            }

            ImGui::Dummy(ImVec2(joystick_size, joystick_size));
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            ImGui::BeginDisabled(is_calibrating);
            if (ImGui::Button("Calibrate") && !is_calibrating) {
                // ps2plman_disable_enable_configuration(false, NULL);
                is_calibrating = true;
                BeginCaptureGamepad();

                for (int i = 0; i < NUM_JOYSTICK_AXIS_RANGES; i++) {
                    joystick_axis_range_remappings[i] = 127;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset##JoystickAxisRangeMapping")) {
                for (int i = 0; i < NUM_JOYSTICK_AXIS_RANGES / 3; i++) {
                    joystick_axis_range_remappings[i*3 + 0] = 0;
                    joystick_axis_range_remappings[i*3 + 1] = 127;
                    joystick_axis_range_remappings[i*3 + 2] = 255;
                }
            }
            ImGui::EndDisabled();

            if (is_calibrating) {
                ImGui::Widgets::GamePadIcon(ImGui::Widgets::WidgetGamePadIconType_Square); ImGui::SameLine();
                    ImGui::Text("Update centers");

                ImGui::Widgets::GamePadIcon(ImGui::Widgets::WidgetGamePadIconType_Circle); ImGui::SameLine();
                    ImGui::Text("Finish calibration");
            }

            if (is_calibrating && state->pad_status.buttonsNew & PAD_SQUARE) {
                joystick_axis_range_remappings[JSAxisRange_LeftXCenter] = state->pad_status.pad.ljoy_h;
                joystick_axis_range_remappings[JSAxisRange_LeftYCenter] = state->pad_status.pad.ljoy_v;
                joystick_axis_range_remappings[JSAxisRange_RightXCenter] = state->pad_status.pad.rjoy_h;
                joystick_axis_range_remappings[JSAxisRange_RightYCenter] = state->pad_status.pad.rjoy_v;
            }

            if (is_calibrating && state->pad_status.buttonsNew & PAD_CIRCLE) {
                is_calibrating = false;
                EndCaptureGamepad();
                // ps2plman_disable_enable_configuration(true, NULL);
            }

            if (is_calibrating) {
                if (state->pad_status.pad.ljoy_h < joystick_axis_range_remappings[JSAxisRange_LeftXMin])
                    joystick_axis_range_remappings[JSAxisRange_LeftXMin] = state->pad_status.pad.ljoy_h;
                if (state->pad_status.pad.ljoy_h > joystick_axis_range_remappings[JSAxisRange_LeftXMax])
                    joystick_axis_range_remappings[JSAxisRange_LeftXMax] = state->pad_status.pad.ljoy_h;
                if (state->pad_status.pad.ljoy_v < joystick_axis_range_remappings[JSAxisRange_LeftYMin])
                    joystick_axis_range_remappings[JSAxisRange_LeftYMin] = state->pad_status.pad.ljoy_v;
                if (state->pad_status.pad.ljoy_v > joystick_axis_range_remappings[JSAxisRange_LeftYMax])
                    joystick_axis_range_remappings[JSAxisRange_LeftYMax] = state->pad_status.pad.ljoy_v;
                    
                if (state->pad_status.pad.rjoy_h < joystick_axis_range_remappings[JSAxisRange_RightXMin])
                    joystick_axis_range_remappings[JSAxisRange_RightXMin] = state->pad_status.pad.rjoy_h;
                if (state->pad_status.pad.rjoy_h > joystick_axis_range_remappings[JSAxisRange_RightXMax])
                    joystick_axis_range_remappings[JSAxisRange_RightXMax] = state->pad_status.pad.rjoy_h;
                if (state->pad_status.pad.rjoy_v < joystick_axis_range_remappings[JSAxisRange_RightYMin])
                    joystick_axis_range_remappings[JSAxisRange_RightYMin] = state->pad_status.pad.rjoy_v;
                if (state->pad_status.pad.rjoy_v > joystick_axis_range_remappings[JSAxisRange_RightYMax])
                    joystick_axis_range_remappings[JSAxisRange_RightYMax] = state->pad_status.pad.rjoy_v;
            }
        }
        ImGui::EndGroup();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2, 2));
        ImGui::BeginDisabled();
        if (ImGui::BeginTable("Joystick Axis Range Remapping", 8)) {
            uint8_t first_min = 0, first_max = 127, second_min = 128, second_max = 255;

            ImGui::TableSetupColumn("##LX-Label", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##LX-Value", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##LY-Label", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##LY-Value", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##RX-Label", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##RX-Value", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##RY-Label", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##RY-Value", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("LX-");
            ImGui::TableNextColumn(); ImGui::DragScalar("##LX-", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_LeftXMin], 1.0F, &first_min, &first_max);
            ImGui::TableNextColumn(); ImGui::Text("LY-");
            ImGui::TableNextColumn(); ImGui::DragScalar("##LY-", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_LeftYMin], 1.0F, &first_min, &first_max);
            ImGui::TableNextColumn(); ImGui::Text("RX-");
            ImGui::TableNextColumn(); ImGui::DragScalar("##RX-", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_RightXMin], 1.0F, &first_min, &first_max);
            ImGui::TableNextColumn(); ImGui::Text("RY-");
            ImGui::TableNextColumn(); ImGui::DragScalar("##RY-", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_RightYMin], 1.0F, &first_min, &first_max);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("LX");
            ImGui::TableNextColumn(); ImGui::DragScalar("##LX", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_LeftXCenter], 1.0F, &joystick_axis_range_remappings[JSAxisRange_LeftXMin], &joystick_axis_range_remappings[JSAxisRange_LeftXMax]);
            ImGui::TableNextColumn(); ImGui::Text("LY");
            ImGui::TableNextColumn(); ImGui::DragScalar("##LY", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_LeftYCenter], 1.0F, &joystick_axis_range_remappings[JSAxisRange_LeftYMin], &joystick_axis_range_remappings[JSAxisRange_LeftYMax]);
            ImGui::TableNextColumn(); ImGui::Text("RX");
            ImGui::TableNextColumn(); ImGui::DragScalar("##RX", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_RightXCenter], 1.0F, &joystick_axis_range_remappings[JSAxisRange_RightXMin], &joystick_axis_range_remappings[JSAxisRange_RightXMax]);
            ImGui::TableNextColumn(); ImGui::Text("RY");
            ImGui::TableNextColumn(); ImGui::DragScalar("##RY", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_RightYCenter], 1.0F, &joystick_axis_range_remappings[JSAxisRange_RightYMin], &joystick_axis_range_remappings[JSAxisRange_RightYMax]);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("LX+");
            ImGui::TableNextColumn(); ImGui::DragScalar("##LX+", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_LeftXMax], 1.0F, &second_min, &second_max);
            ImGui::TableNextColumn(); ImGui::Text("LY+");
            ImGui::TableNextColumn(); ImGui::DragScalar("##LY+", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_LeftYMax], 1.0F, &second_min, &second_max);
            ImGui::TableNextColumn(); ImGui::Text("RX+");
            ImGui::TableNextColumn(); ImGui::DragScalar("##RX+", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_RightXMax], 1.0F, &second_min, &second_max);
            ImGui::TableNextColumn(); ImGui::Text("RY+");
            ImGui::TableNextColumn(); ImGui::DragScalar("##RY+", ImGuiDataType_U8, &joystick_axis_range_remappings[JSAxisRange_RightYMax], 1.0F, &second_min, &second_max);

            ImGui::EndTable();
        }
        ImGui::EndDisabled();
        ImGui::PopStyleVar();
        ImGui::EndDisabled();

        ImGui::TreePop();
        ImGui::Separator();
    }
}

void app_configuration_joysticks(ImGuiIO &io, configurator_state *state) {
    if (ImGui::TreeNodeEx("Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {
        app_configuration_joysticks_deadzones(io, state);
        app_configuration_joysticks_digital_mode(io, state);
        app_configuration_joysticks_axis_range_remapping(io, state);

        ImGui::TreePop();
    }
}

void app_section_configuration(ImGuiIO &io, configurator_state *state) {
    if (!state->current_controller || !state->current_controller->connected) {
        return;
    }

    app_configuration_buttons(io, state);
    app_configuration_joysticks(io, state);
}