#include "app.h"

#include "../ui/fonts/playstation.h"

#include <math.h>

void app_section_test(ImGuiIO &io, configurator_state *state) {
    if (!state->current_controller || !state->current_controller->connected) {
        return;
    }

    static bool is_testing = false;
    
    // Flags for actuator controls
    bool dpad_active = (
        state->pad_status.IsButtonDown(PAD_UP) ||
        state->pad_status.IsButtonDown(PAD_DOWN) ||
        state->pad_status.IsButtonDown(PAD_LEFT) ||
        state->pad_status.IsButtonDown(PAD_RIGHT)
    );

    float joystick_lx = state->pad_status.GetJoystickAxis(JSLeftX);
    float joystick_ly = state->pad_status.GetJoystickAxis(JSLeftY);
    float joystick_distance = sqrtf(joystick_lx*joystick_lx + joystick_ly*joystick_ly);
    uint8_t joystick_power = (uint8_t)(joystick_distance * 0xFF);
    bool joystick_active = joystick_power > 0x40; // From the Curious Inventor PS2 document, 0x40 was the practical minimum for spinning the motors

    // Enter/exit testing mode via L3+R3 -- to prevent app navigation while testing
    if ((state->pad_status.IsButtonPressed(PAD_L3) && state->pad_status.IsButtonPressed(PAD_R3)) || 
        (state->pad_status.IsButtonPressed(PAD_L3) && state->pad_status.IsButtonDown(PAD_R3)) ||
        (state->pad_status.IsButtonDown(PAD_L3) && state->pad_status.IsButtonPressed(PAD_R3))) 
    {
        is_testing = !is_testing;

        if (is_testing) {
            PS2Plus::Graphics::PauseGamepadNav();
        } else {
            PS2Plus::Graphics::ResumeGamepadNav();
        }
    } 

    if (is_testing) {
        // Drive the rumble actuators when triangle is held
        if (state->pad_status.IsButtonDown(PAD_TRIANGLE)) {
            if (!state->pad_status.IsRumbleActuatorSmallActive() && dpad_active) {
                // Small actuator, enabled when any d-pad button is down
                state->pad_status.SetRumbleActuatorSmall(true);
            } else if (state->pad_status.IsRumbleActuatorSmallActive() && !dpad_active) {
                // Small actuator, disabled when all d-pad buttons are up
                state->pad_status.SetRumbleActuatorSmall(false);
            }


            if (joystick_active) {
                // Large actuator, enabled when the left joystick is pulled sufficiently in any direction
                state->pad_status.SetRumbleActuatorLargePower(joystick_power);
            } else if (state->pad_status.GetRumbleActuatorLargePower() && !joystick_active) {
                // Large actuator, disabled when the left joystick is not pulled succiently enough
                state->pad_status.SetRumbleActuatorLargePower(0);
            }
        } else {
            if (state->pad_status.IsRumbleActuatorSmallActive()) {
                state->pad_status.SetRumbleActuatorSmall(false);
            }

            if (state->pad_status.GetRumbleActuatorLargePower()) {
                state->pad_status.SetRumbleActuatorLargePower(0);
            }
        }
    }

    ImGui::BeginChild("GamePadTesterSection", ImVec2(0, 0), true, ImGuiWindowFlags_NavFlattened);
    {
        if (ImGui::BeginTable("#GamePadTesterControls", 2, ImGuiTableFlags_Borders)) {
            ImGui::TableSetupColumn("Buttons##Buttons-Label", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Description##Buttons-Value", ImGuiTableColumnFlags_WidthStretch);

            const char *icon_l3_button = state->pad_status.IsButtonDown(PAD_L3) ? ICON_PLAYSTATION_L3_BUTTON_ALT : ICON_PLAYSTATION_L3_BUTTON;
            const char *icon_r3_button = state->pad_status.IsButtonDown(PAD_R3) ? ICON_PLAYSTATION_R3_BUTTON_ALT : ICON_PLAYSTATION_R3_BUTTON;
            const char *icon_triangle_button = (is_testing && state->pad_status.IsButtonDown(PAD_TRIANGLE)) ? ICON_PLAYSTATION_TRIANGLE_BUTTON_ALT : ICON_PLAYSTATION_TRIANGLE_BUTTON;
            const char *icon_left_joystick = (is_testing && joystick_active) ? ICON_PLAYSTATION_LEFT_STICK_ALT : ICON_PLAYSTATION_LEFT_STICK;
            const char *icon_dpad = (is_testing && dpad_active) ? ICON_PLAYSTATION_DS3_DPAD_FULL_ALT : ICON_PLAYSTATION_DS3_DPAD_FULL;

            const ImVec4& disabled_color = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
            const ImVec4& standard_color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
            const ImVec4& triangle_color = is_testing ? ICON_PLAYSTATION_COLOR_TRIANGLE : disabled_color;
            const ImVec4& text_color = is_testing ? standard_color : disabled_color;

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); 
            ImGui::Text(icon_l3_button); ImGui::SameLine(0.f, 0.f); 
            ImGui::Text(" + "); ImGui::SameLine(0.f, 0.f); 
            ImGui::Text(icon_r3_button);
            ImGui::TableNextColumn(); 
            ImGui::TextWrapped("Begin or end button testing");

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); 
            ImGui::TextColored(triangle_color, icon_triangle_button); ImGui::SameLine(0.f, 0.f); 
            ImGui::TextColored(text_color, " + "); ImGui::SameLine(0.f, 0.f); 
            ImGui::TextColored(text_color, icon_left_joystick);
            ImGui::TableNextColumn(); 
            ImGui::TextColored(text_color, "Actuate large rumble motor");

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); 
            ImGui::TextColored(triangle_color, icon_triangle_button); ImGui::SameLine(0.f, 0.f); 
            ImGui::TextColored(text_color, " + "); ImGui::SameLine(0.f, 0.f); 
            ImGui::TextColored(text_color, icon_dpad);
            ImGui::TableNextColumn(); 
            ImGui::TextColored(text_color, "Actuate small rumble motor");

            ImGui::EndTable();
        }

        ImGui::Separator();
        PS2Plus::UI::GamePadVisualizer("GamepadTester", state->pad_status, ImGui::GetContentRegionAvail(), !is_testing);
    }
    ImGui::EndChild();
    
    
}
