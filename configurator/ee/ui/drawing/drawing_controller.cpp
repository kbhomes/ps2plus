#include "drawing.h"

static const ImU32 ColorGrayLight = IM_COL32(0xC0, 0xC0, 0xC0, 0xFF);
static const ImU32 ColorGrayMedium = IM_COL32(0x60, 0x60, 0x60, 0xFF);
static const ImU32 ColorGrayDark = IM_COL32(0x20, 0x20, 0x20, 0xFF);
static const ImU32 ColorWhite = IM_COL32_WHITE;

void draw_controller_joystick(ImDrawList *draw_list, const ImVec2& center, float size, bool pressed, ImU8 x, ImU8 y, bool bordered, ImU32 border_color, bool disabled) {
    // Draw the joystick boundaries and axes
    if (bordered) {
        imgui_draw_rect(draw_list, false, ImVec2(center.x - size - 2, center.y - size - 2), ImVec2(center.x + size + 2, center.y + size + 2), border_color, size * 0.1, 0, 2);
    } 
    
    if (!disabled) {
        imgui_draw_rect(draw_list, true, ImVec2(center.x - size, center.y - size), ImVec2(center.x + size, center.y + size), pressed ? ColorWhite : ColorGrayDark, size * 0.1);
    }

    imgui_draw_line(draw_list, ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size - 1), pressed ? ColorGrayLight : ColorGrayMedium);
    imgui_draw_line(draw_list, ImVec2(center.x - size, center.y), ImVec2(center.x + size - 1, center.y), pressed ? ColorGrayLight : ColorGrayMedium);

    // Draw the current X and Y positions
    float horizontal = x/256.f * 2 - 1;
    float vertical = y/256.f * 2 - 1;
    float cursorSize = size * 0.1;
    ImVec2 cursorCenter = ImVec2(center.x + size*horizontal, center.y + size*vertical);
    imgui_draw_line(draw_list, ImVec2(cursorCenter.x, cursorCenter.y - cursorSize), ImVec2(cursorCenter.x, cursorCenter.y + cursorSize), pressed ? ColorGrayDark : ColorWhite);
    imgui_draw_line(draw_list, ImVec2(cursorCenter.x - cursorSize, cursorCenter.y), ImVec2(cursorCenter.x + cursorSize, cursorCenter.y), pressed ? ColorGrayDark : ColorWhite);
}