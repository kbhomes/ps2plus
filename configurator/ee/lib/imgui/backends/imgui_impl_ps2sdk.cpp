// dear imgui: Platform Backend for PlayStation 2 consoles using ps2sdk
// This needs to be used along with the PS2 gsKit renderer

#include "../imgui.h"
#include "imgui_impl_ps2sdk.h"

// TODO: ps2sdk includes
#include <gsKit.h>
#include <dmaKit.h>
#include <tamtypes.h>
#include <ps2sdkapi.h>
#include <libpad.h>
#include <math.h>
#include <time.h>

// gsKit Data
struct ImGui_ImplPs2Sdk_Data
{
    GSGLOBAL* Global;
    float Time;

    ImGui_ImplPs2Sdk_Data()   { memset(this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear ImGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear ImGui context + multiple windows) instead of multiple Dear ImGui contexts.
static ImGui_ImplPs2Sdk_Data* ImGui_ImplPs2Sdk_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplPs2Sdk_Data*)ImGui::GetIO().BackendPlatformUserData : NULL;
}

// Functions

static float GetTime() 
{
    return (float)(clock()) / CLOCKS_PER_SEC;
}

static bool ImGui_ImplPs2Sdk_Init(GSGLOBAL* global)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == NULL && "Already initialized a platform backend!");

    // Setup backend capabilities flags
    ImGui_ImplPs2Sdk_Data* bd = IM_NEW(ImGui_ImplPs2Sdk_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_ps2sdk";
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->Global = global;

    io.SetClipboardTextFn = NULL;
    io.GetClipboardTextFn = NULL;
    io.ClipboardUserData = NULL;

    return true;
}

bool ImGui_ImplPs2Sdk_InitForGsKit(GSGLOBAL* global)
{
    return ImGui_ImplPs2Sdk_Init(global);
}

void ImGui_ImplPs2Sdk_Shutdown()
{
    ImGui_ImplPs2Sdk_Data* bd = ImGui_ImplPs2Sdk_GetBackendData();
    IM_ASSERT(bd != NULL && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    io.BackendPlatformName = NULL;
    io.BackendPlatformUserData = NULL;
    IM_DELETE(bd);
}

void ImGui_ImplPs2Sdk_NewFrame()
{
    ImGui_ImplPs2Sdk_Data* bd = ImGui_ImplPs2Sdk_GetBackendData();
    IM_ASSERT(bd != NULL && "Did you call ImGui_ImplPs2Sdk_Init()?");
    ImGuiIO& io = ImGui::GetIO();

    // Update the framebuffer to handle resizing
    io.DisplaySize = ImVec2((float)bd->Global->Width, (float)bd->Global->Height);
    io.DisplayFramebufferScale = ImVec2(1.0, 1.0);
    
    // Update time
    float now = GetTime();
    io.DeltaTime = now - bd->Time;
    bd->Time = now;
}

void ImGui_ImplPs2Sdk_UpdateGamepad(const padButtonStatus *pad)
{
    ImGui_ImplPs2Sdk_Data* bd = ImGui_ImplPs2Sdk_GetBackendData();
    IM_ASSERT(bd != NULL && "Did you call ImGui_ImplPs2Sdk_Init()?");
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
        return;

    if (!pad) {
        // Controller is not active, so disable it in imgui
        io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
        return;
    }
    
    // Invert active-low button states
    u16 buttons = pad->btns ^ 0xFFFF;

    #define MAP_DIGITAL_BUTTON(GAMEPAD_KEY, BUTTON_MASK)    do { io.AddKeyEvent(GAMEPAD_KEY, (buttons & BUTTON_MASK) ? 1.0f : 0.0f); } while (0);
    #define MAP_ANALOG(GAMEPAD_KEY, AXIS_VAL, V0, V1, DZF)  do { float vn = (float)(AXIS_VAL - V0) / (float)(V1 - V0); if (vn < 0.0f) vn = 0.0f; if (vn > 1.0f) vn = 1.0f; io.AddKeyAnalogEvent(GAMEPAD_KEY, vn > DZF, vn); } while (0);

    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadFaceDown,    PAD_CROSS);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadFaceRight,   PAD_CIRCLE);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadFaceLeft,    PAD_SQUARE);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadFaceUp,      PAD_TRIANGLE);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadDpadLeft,    PAD_LEFT);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadDpadRight,   PAD_RIGHT);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadDpadUp,      PAD_UP);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadDpadDown,    PAD_DOWN);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadL1,          PAD_L1);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadR1,          PAD_R1);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadL2,          PAD_L2);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadR2,          PAD_R2);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadL3,          PAD_L3);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadR3,          PAD_R3);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadStart,       PAD_START);
    MAP_DIGITAL_BUTTON(ImGuiKey_GamepadBack,        PAD_SELECT);

    MAP_ANALOG(ImGuiKey_GamepadLStickLeft, pad->ljoy_h, 0x7F, 0x00, 0.15);
    MAP_ANALOG(ImGuiKey_GamepadLStickRight, pad->ljoy_h, 0x80, 0xFF, 0.15);
    MAP_ANALOG(ImGuiKey_GamepadLStickUp, pad->ljoy_v, 0x7F, 0x00, 0.15);
    MAP_ANALOG(ImGuiKey_GamepadLStickDown, pad->ljoy_v, 0x80, 0xFF, 0.15);

    #undef MAP_DIGITAL_BUTTON
    #undef MAP_ANALOG

    #define ANALOG_CLAMP_DEADZONE(AXIS_VAL, DEAD_ZONE) ((AXIS_VAL >= DEAD_ZONE || AXIS_VAL <= -DEAD_ZONE) ? AXIS_VAL : 0)

    float rstickX = ANALOG_CLAMP_DEADZONE(pad->rjoy_h / 255.f * 2.f - 1.f, 0.15);
    float rstickY = ANALOG_CLAMP_DEADZONE(pad->rjoy_v / 255.f * 2.f - 1.f, 0.15);

    // Show and adjust the mouse cursor if the right analog stick has been touched
    if (rstickX || rstickY) {
        io.MouseDrawCursor = true;
        io.AddMousePosEvent(
            fminf(io.DisplaySize.x - 1, fmaxf(0, io.MousePos.x + rstickX * 8)), 
            fminf(io.DisplaySize.y - 1, fmaxf(0, io.MousePos.y + rstickY * 8)));
    }

    // Only allow emulated mouse clicks if we are in cursor mode
    if (io.MouseDrawCursor) {
        io.AddMouseButtonEvent(ImGuiMouseButton_Left, (buttons & PAD_R3) | (buttons & PAD_R2));
    }

    // Hide the mouse cursor if the D-pad has been touched
    if (buttons & (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN)) {
        io.MouseDrawCursor = false;
    }

    // Indicate that the gamepad is present
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
}