#pragma once

#include <imgui/imgui.h>

namespace PS2Plus::Components {
    void IconCheckmark(float size, ImU32 color);
    void IconError(float size, ImU32 color);
    void IconWarning(float size, ImU32 color);
    void IconArrow(float size, ImU32 color, ImGuiDir dir);
}