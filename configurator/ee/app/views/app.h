#pragma once

#include "app/state.h"

#include <imgui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace PS2Plus::App::Views {
void MainView();
void NoPS2PlusConnectedMessage();
void BeginCaptureGamepad();
void EndCaptureGamepad();
} // namespace PS2Plus::App::Views