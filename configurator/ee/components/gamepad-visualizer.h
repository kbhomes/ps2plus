#pragma once

#include "ui/drawing/drawing.h"
#include "ui/gfx.h"
#include "ui/pad.h"

#include <imgui/imgui.h>

namespace PS2Plus::Components {
    void GamepadVisualizer(const char *id, const PS2Plus::Gamepad::PadStatus &pad, const ImVec2& size = { 300.f, 150.f }, bool disabled = false);
}