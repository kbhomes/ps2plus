#include "app.h"

#include "ui/fonts/playstation.h"
#include "components/gamepad-visualizer.h"

#include <math.h>

void app_section_test(ImGuiIO &io, configurator_state *state) {
    static bool is_testing = false;
    
    // Flags for actuator controls
    bool dpad_active = (
        state->current_pad.IsButtonDown(PAD_UP) ||
        state->current_pad.IsButtonDown(PAD_DOWN) ||
        state->current_pad.IsButtonDown(PAD_LEFT) ||
        state->current_pad.IsButtonDown(PAD_RIGHT)
    );

    float joystick_lx = state->current_pad.GetJoystickAxis(JSLeftX);
    float joystick_ly = state->current_pad.GetJoystickAxis(JSLeftY);
    float joystick_distance = sqrtf(joystick_lx*joystick_lx + joystick_ly*joystick_ly);
    uint8_t joystick_power = (uint8_t)(fminf(joystick_distance, 1.f) * 0xFF);
    bool joystick_active = joystick_power > 0x40; // From the Curious Inventor PS2 document, 0x40 was the practical minimum for spinning the motors

    // Enter/exit testing mode via L3+R3 -- to prevent app navigation while testing
    if ((state->current_pad.IsButtonPressed(PAD_L3) && state->current_pad.IsButtonPressed(PAD_R3)) || 
        (state->current_pad.IsButtonPressed(PAD_L3) && state->current_pad.IsButtonDown(PAD_R3)) ||
        (state->current_pad.IsButtonDown(PAD_L3) && state->current_pad.IsButtonPressed(PAD_R3))) 
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
        if (state->current_pad.IsButtonDown(PAD_TRIANGLE)) {
            if (!state->current_pad.IsRumbleActuatorSmallActive() && dpad_active) {
                // Small actuator, enabled when any d-pad button is down
                state->current_pad.SetRumbleActuatorSmall(true);
            } else if (state->current_pad.IsRumbleActuatorSmallActive() && !dpad_active) {
                // Small actuator, disabled when all d-pad buttons are up
                state->current_pad.SetRumbleActuatorSmall(false);
            }


            if (joystick_active) {
                // Large actuator, enabled when the left joystick is pulled sufficiently in any direction
                state->current_pad.SetRumbleActuatorLargePower(joystick_power);
            } else if (state->current_pad.GetRumbleActuatorLargePower() && !joystick_active) {
                // Large actuator, disabled when the left joystick is not pulled succiently enough
                state->current_pad.SetRumbleActuatorLargePower(0);
            }
        } else {
            if (state->current_pad.IsRumbleActuatorSmallActive()) {
                state->current_pad.SetRumbleActuatorSmall(false);
            }

            if (state->current_pad.GetRumbleActuatorLargePower()) {
                state->current_pad.SetRumbleActuatorLargePower(0);
            }
        }
    }

    ImGui::BeginChild("GamePadTesterSection", ImVec2(0, 0), true, ImGuiWindowFlags_NavFlattened);
    {
        if (ImGui::BeginTable("#GamePadTesterControls", 2, ImGuiTableFlags_Borders)) {
            ImGui::TableSetupColumn("Buttons##Buttons-Label", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Description##Buttons-Value", ImGuiTableColumnFlags_WidthStretch);

            const char *icon_l3_button = state->current_pad.IsButtonDown(PAD_L3) ? ICON_PLAYSTATION_L3_BUTTON_ALT : ICON_PLAYSTATION_L3_BUTTON;
            const char *icon_r3_button = state->current_pad.IsButtonDown(PAD_R3) ? ICON_PLAYSTATION_R3_BUTTON_ALT : ICON_PLAYSTATION_R3_BUTTON;
            const char *icon_triangle_button = (is_testing && state->current_pad.IsButtonDown(PAD_TRIANGLE)) ? ICON_PLAYSTATION_TRIANGLE_BUTTON_ALT : ICON_PLAYSTATION_TRIANGLE_BUTTON;
            const char *icon_left_joystick = (is_testing && joystick_active) ? ICON_PLAYSTATION_LEFT_STICK_ALT : ICON_PLAYSTATION_LEFT_STICK;
            const char *icon_dpad = (is_testing && dpad_active) ? ICON_PLAYSTATION_DS3_DPAD_FULL_ALT : ICON_PLAYSTATION_DS3_DPAD_FULL;

            const ImVec4& triangle_color = is_testing ? ICON_PLAYSTATION_COLOR_TRIANGLE : ImGui::GetStyleColorVec4(ImGuiCol_Text);

            ImGui::BeginDisabled(!is_testing);

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
            ImGui::Text(" + "); ImGui::SameLine(0.f, 0.f); 
            ImGui::Text(icon_left_joystick);
            ImGui::TableNextColumn(); 
            ImGui::Text("Actuate large rumble motor"); ImGui::SameLine();
            ImGui::SetWindowFontScale(0.7f);
            ImGui::ProgressBar((float)state->current_pad.GetRumbleActuatorLargePower() / 0xFF);
            ImGui::SetWindowFontScale(1.f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); 
            ImGui::TextColored(triangle_color, icon_triangle_button); ImGui::SameLine(0.f, 0.f); 
            ImGui::Text(" + "); ImGui::SameLine(0.f, 0.f); 
            ImGui::Text(icon_dpad);
            ImGui::TableNextColumn(); 
            ImGui::Text("Actuate small rumble motor"); ImGui::SameLine();
            ImGui::SetWindowFontScale(0.7f);
            ImGui::ProgressBar(state->current_pad.IsRumbleActuatorSmallActive() ? 1.f : 0.f);
            ImGui::SetWindowFontScale(1.f);
            
            ImGui::EndDisabled();

            ImGui::EndTable();
        }

        ImGui::Separator();
        PS2Plus::Components::GamepadVisualizer("GamepadTester", state->current_pad, ImGui::GetContentRegionAvail(), !is_testing);
    }
    ImGui::EndChild();
    
    
}
