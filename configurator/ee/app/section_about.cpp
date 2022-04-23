#include "app.h"

#include "ui/fonts/playstation.h"
#include "ui/widgets/status/status.h"
#include "ui/widgets/status/loading-progress.h"

#include <math.h>

void app_section_about(ImGuiIO &io, configurator_state *state) {
    PS2Plus::UI::StatusText("Firmware updated!", PS2Plus::UI::StatusType_Success);
    PS2Plus::UI::StatusText("Update taking longer than expected", PS2Plus::UI::StatusType_Warning);
    PS2Plus::UI::StatusText("Error occurred during update", PS2Plus::UI::StatusType_Error);
    PS2Plus::UI::StatusText("Rebooting controller", PS2Plus::UI::StatusType_Loading);
    PS2Plus::UI::LoadingProgress("##Progress", fmodf(ImGui::GetTime(), 5.f) / 5.f, "%0.f%%");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_TRIANGLE, state->pad_status.IsButtonDown(PAD_TRIANGLE) 
        ? ICON_PLAYSTATION_TRIANGLE_BUTTON_ALT 
        : ICON_PLAYSTATION_TRIANGLE_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Options");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_CIRCLE, state->pad_status.IsButtonDown(PAD_CIRCLE) 
        ? ICON_PLAYSTATION_CIRCLE_BUTTON_ALT 
        : ICON_PLAYSTATION_CIRCLE_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Cancel");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_CROSS, state->pad_status.IsButtonDown(PAD_CROSS) 
        ? ICON_PLAYSTATION_CROSS_BUTTON_ALT 
        : ICON_PLAYSTATION_CROSS_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Select");

    ImGui::TextColored(ICON_PLAYSTATION_COLOR_SQUARE, state->pad_status.IsButtonDown(PAD_SQUARE) 
        ? ICON_PLAYSTATION_SQUARE_BUTTON_ALT 
        : ICON_PLAYSTATION_SQUARE_BUTTON); 
    ImGui::SameLine(); 
    ImGui::Text("Mark");
}