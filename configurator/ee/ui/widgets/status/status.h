#pragma once

#include <imgui/imgui.h>

namespace PS2Plus::UI {
    enum StatusType {
        StatusType_Success,
        StatusType_Error,
        StatusType_Warning,
        StatusType_Loading,
    };

    void StatusText(const char *label, StatusType status, ...);
}