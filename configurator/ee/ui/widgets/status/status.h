#pragma once

#include <imgui/imgui.h>

namespace ImGui::Widgets {
    enum StatusType {
        StatusType_Success,
        StatusType_Error,
        StatusType_Warning,
        StatusType_Loading,
    };

    void StatusText(const char *label, StatusType status, ...);
}