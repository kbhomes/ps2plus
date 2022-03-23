#pragma once

#include <imgui/imgui.h>

namespace PS2Plus::UI::Icons {
    void Checkmark(float size, ImU32 color);
    void Error(float size, ImU32 color);
    void Warning(float size, ImU32 color);
    void Arrow(float size, ImU32 color, ImGuiDir dir);
}