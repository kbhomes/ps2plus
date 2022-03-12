#ifndef UI_GFX_H
#define UI_GFX_H

#include <gsKit.h>
#include <tamtypes.h>
#include "pad.h"

GSGLOBAL *gfx_init(bool hires);
void gfx_imgui_init(GSGLOBAL *gsGlobal);
void gfx_render_clear(GSGLOBAL *gsGlobal, u64 color);
void gfx_render_begin(GSGLOBAL *gsGlobal, bool hires);
void gfx_render_end(GSGLOBAL *gsGlobal, bool hires);
void gfx_update_pad(GSGLOBAL *gsGlobal, PadStatus *pad);

#endif /* UI_GFX_H */