#include "app.h"

#include "ui/widgets/status/status.h"
#include "ui/widgets/status/loading-progress.h"

#include <math.h>

void app_section_about(ImGuiIO &io, configurator_state *state) {
    PS2Plus::UI::StatusText("Firmware updated!", PS2Plus::UI::StatusType_Success);
    PS2Plus::UI::StatusText("Update taking longer than expected", PS2Plus::UI::StatusType_Warning);
    PS2Plus::UI::StatusText("Error occurred during update", PS2Plus::UI::StatusType_Error);
    PS2Plus::UI::StatusText("Rebooting controller", PS2Plus::UI::StatusType_Loading);
    PS2Plus::UI::LoadingProgress("##Progress", fmodf(ImGui::GetTime(), 5.f) / 5.f, "%0.f%%");
}