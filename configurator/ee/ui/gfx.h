#ifndef UI_GFX_H
#define UI_GFX_H

#include <gsKit.h>
#include <stdint.h>
#include <tamtypes.h>
#include "pad.h"

namespace PS2Plus::Graphics {
    void Initialize(bool hiresFlag = false);
    void BeginFrame(uint64_t color = GS_SETREG_RGBA(0, 0, 0, 0xFF));
    void EndFrame();
    void UpdateGamepad(const PS2Plus::Gamepad::PadStatus& pad);
};

#endif /* UI_GFX_H */