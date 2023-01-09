#pragma once

#include <imgui/imgui.h>

namespace PS2Plus::Components {
    enum StatusType {
        kStatusSuccess,
        kStatusError,
        kStatusWarning,
        kStatusLoading,
    };

    void StatusText(const char *label, StatusType status, ...);
}