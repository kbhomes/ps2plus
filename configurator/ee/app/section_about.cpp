#include "app.h"

#include "ui/fonts/playstation.h"
#include "ui/widgets/status/status.h"
#include "ui/widgets/status/loading-progress.h"

#include <math.h>

void video_mode_selection(configurator_state *state) {
    PS2Plus::Graphics::VideoMode previousMode = state->video_mode;
    const char *modeDisplay = PS2Plus::Graphics::VideoModeStrings[state->video_mode];
    if (ImGui::BeginCombo("Video Mode", modeDisplay)) {
        for (auto const &entry : PS2Plus::Graphics::VideoModeStrings) {
            const bool isSelected = entry.first == state->video_mode;
            if (ImGui::Selectable(entry.second, isSelected)) {
                state->video_mode = entry.first;
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (state->video_mode != previousMode) {
        PS2Plus::Graphics::SetVideoMode(state->video_mode);
    }
}

void app_section_about(ImGuiIO &io, configurator_state *state) {
    video_mode_selection(state);
    ImGui::Separator();

    ImGui::ShowStyleEditor();

    PS2Plus::UI::StatusText("Firmware updated!", PS2Plus::UI::StatusType_Success);
    PS2Plus::UI::StatusText("Update taking longer than expected", PS2Plus::UI::StatusType_Warning);
    PS2Plus::UI::StatusText("Error occurred during update", PS2Plus::UI::StatusType_Error);
    PS2Plus::UI::StatusText("Rebooting controller", PS2Plus::UI::StatusType_Loading);
    PS2Plus::UI::LoadingProgress("##Progress", fmodf(ImGui::GetTime(), 5.f) / 5.f, "%0.f%%");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_TRIANGLE, state->pad_summary.IsButtonDown(PAD_TRIANGLE) 
        ? ICON_PLAYSTATION_TRIANGLE_BUTTON_ALT 
        : ICON_PLAYSTATION_TRIANGLE_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Options");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_CIRCLE, state->pad_summary.IsButtonDown(PAD_CIRCLE) 
        ? ICON_PLAYSTATION_CIRCLE_BUTTON_ALT 
        : ICON_PLAYSTATION_CIRCLE_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Cancel");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_CROSS, state->pad_summary.IsButtonDown(PAD_CROSS) 
        ? ICON_PLAYSTATION_CROSS_BUTTON_ALT 
        : ICON_PLAYSTATION_CROSS_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Select");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_SQUARE, state->pad_summary.IsButtonDown(PAD_SQUARE) 
        ? ICON_PLAYSTATION_SQUARE_BUTTON_ALT 
        : ICON_PLAYSTATION_SQUARE_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Mark");

    ImGui::Text(state->pad_summary.IsButtonDown(PAD_START) 
        ? ICON_PLAYSTATION_START_BUTTON_ALT
        : ICON_PLAYSTATION_START_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Start");

    ImGui::Text(state->pad_summary.IsButtonDown(PAD_SELECT) 
        ? ICON_PLAYSTATION_SELECT_BUTTON_ALT 
        : ICON_PLAYSTATION_SELECT_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Select");
}