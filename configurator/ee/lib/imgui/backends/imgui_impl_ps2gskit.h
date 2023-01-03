// dear imgui: Renderer Backend for PS2 gsKit
// This needs to be used along with the PS2SDK backend

#pragma once
#include "../imgui.h"      // IMGUI_IMPL_API
#include <gsKit.h>

IMGUI_IMPL_API bool     ImGui_ImplPs2GsKit_Init(GSGLOBAL *global);
IMGUI_IMPL_API void     ImGui_ImplPs2GsKit_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplPs2GsKit_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplPs2GsKit_RenderDrawData(ImDrawData* draw_data, ImVec2 pixelOffset = ImVec2(-0.5f, -0.5f));

// Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool     ImGui_ImplPs2GsKit_CreateFontsTexture();
IMGUI_IMPL_API void     ImGui_ImplPs2GsKit_DestroyFontsTexture();
IMGUI_IMPL_API bool     ImGui_ImplPs2GsKit_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplPs2GsKit_DestroyDeviceObjects();
