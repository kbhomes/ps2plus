#pragma once

#include <imgui/imgui.h>
#include <math.h>

void imgui_draw_circle(ImDrawList *draw_list, bool filled, const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.0f);
void imgui_draw_triangle(ImDrawList *draw_list, bool filled, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f);
void imgui_draw_rect(ImDrawList *draw_list, bool filled, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);
void imgui_draw_line(ImDrawList *draw_list, const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);

void draw_controller_joystick(ImDrawList *draw_list, const ImVec2& center, float size, bool pressed, ImU8 x, ImU8 y, bool bordered = false, ImU32 border_color = IM_COL32_WHITE, bool disabled = false);
