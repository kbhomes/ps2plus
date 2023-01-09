#pragma once

#include <imgui/imgui.h>

namespace PS2Plus::Components {
    void LoadingProgress(const char *label, float progress, const char *format = NULL);
}