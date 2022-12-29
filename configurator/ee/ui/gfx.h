#ifndef UI_GFX_H
#define UI_GFX_H

#include <imgui/imgui.h>
#include <gsKit.h>
#include <stdint.h>
#include <tamtypes.h>
#include "pad.h"

namespace PS2Plus::Graphics {
    void Initialize(bool hiresFlag = false);
    void BeginFrame(uint64_t color = GS_SETREG_RGBA(0, 0, 0, 0xFF));
    void EndFrame();
    void UpdateGamepad(const PS2Plus::Gamepad::PadSummary& pad);

    ImFont *GetFontPlayStationLarge();

    void PauseGamepadNav();
    void ResumeGamepadNav();
    bool IsGamepadNavActive();
};

#endif /* UI_GFX_H */