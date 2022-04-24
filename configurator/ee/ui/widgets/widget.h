#pragma once

#include "../drawing/drawing.h"
#include "../gfx.h"
#include "../pad.h"

#include <imgui/imgui.h>

namespace PS2Plus::UI {
    enum WidgetGamePadIconType {
        WidgetGamePadIconType_Triangle,
        WidgetGamePadIconType_Circle,
        WidgetGamePadIconType_Cross,
        WidgetGamePadIconType_Square,
        WidgetGamePadIconType_Start,
        WidgetGamePadIconType_Select,
        WidgetGamePadIconType_DpadUp,
        WidgetGamePadIconType_DpadDown,
        WidgetGamePadIconType_DpadLeft,
        WidgetGamePadIconType_DpadRight,
    };

    void GamePadIcon(WidgetGamePadIconType icon, float height = 0);
    void GamePadVisualizer(const char *id, PS2Plus::Gamepad::PadStatus &pad, const ImVec2& size = { 300.f, 150.f }, bool disabled = false);
    void WindowOverlay(float alpha = 0.75);
    void WindowOverlay(ImU32 color);
    void MouseCursor();
    void HelpMarker(const char* desc);
}