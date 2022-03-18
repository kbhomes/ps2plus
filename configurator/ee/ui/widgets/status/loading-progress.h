#pragma once

#include <imgui/imgui.h>

namespace ImGui::Widgets {
    void LoadingProgress(const char *label, float progress, const char *format = NULL);
}