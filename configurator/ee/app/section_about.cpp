#include "app.h"

#include "ui/widgets/status/status.h"
#include "ui/widgets/status/loading-progress.h"

#include <math.h>

void app_section_about(ImGuiIO &io, configurator_state *state) {
    ImGui::Widgets::StatusText("Firmware updated!", ImGui::Widgets::StatusType_Success);
    ImGui::Widgets::StatusText("Update taking longer than expected", ImGui::Widgets::StatusType_Warning);
    ImGui::Widgets::StatusText("Error occurred during update", ImGui::Widgets::StatusType_Error);
    ImGui::Widgets::StatusText("Rebooting controller", ImGui::Widgets::StatusType_Loading);
    ImGui::Widgets::LoadingProgress("##Progress", fmodf(ImGui::GetTime(), 5.f) / 5.f, "%0.f%%");
}