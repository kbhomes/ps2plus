#pragma once

#include <imgui/imgui.h>

namespace PS2Plus::UI {
    void LoadingProgress(const char *label, float progress, const char *format = NULL);
}