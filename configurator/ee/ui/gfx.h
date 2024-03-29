#ifndef UI_GFX_H
#define UI_GFX_H

#include <imgui/imgui.h>
#include <gsKit.h>
#include <stdint.h>
#include <tamtypes.h>
#include <map>
#include "pad.h"

namespace PS2Plus::Graphics {
    enum VideoMode { Interlaced, Progressive480 };
    static std::map<VideoMode, const char *> VideoModeStrings = {
        { Interlaced, "Standard" },
        { Progressive480, "Progressive (480p)" },
    };

    void Initialize(VideoMode mode);
    void BeginFrame(uint64_t color = GS_SETREG_RGBA(20, 25, 30, 255));
    void EndFrame();
    void UpdateGamepad(const PS2Plus::Gamepad::PadSummary& pad);
    void SetVideoMode(VideoMode mode);

    ImFont *GetFontPlayStationLarge();

    void PauseGamepadNav();
    void ResumeGamepadNav();
    bool IsGamepadGlobalNavActive();
};

#endif /* UI_GFX_H */