// dear imgui: Platform Backend for PlayStation 2 consoles using ps2sdk
// This needs to be used along with the PS2 gsKit renderer

#pragma once
#include "../imgui.h"      // IMGUI_IMPL_API
#include <gsKit.h>
#include <libpad.h>

IMGUI_IMPL_API bool     ImGui_ImplPs2Sdk_InitForGsKit(GSGLOBAL* global);
IMGUI_IMPL_API void     ImGui_ImplPs2Sdk_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplPs2Sdk_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplPs2Sdk_UpdateGamepad(const padButtonStatus *pad);
