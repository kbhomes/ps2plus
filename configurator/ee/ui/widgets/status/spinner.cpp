#include "spinner.h"

#include <math.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#define RADIANS_CIRCLE (M_PI * 2)

void ImGui::Widgets::Spinner(float size, ImU32 color) {
    size *= 0.85;
    ImVec2 bb(size, size);
    
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    float thickness = ImMax(size / 7.5f, 1.0f);
    size -= thickness * 0.5f;
    pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

    ImVec2 center(pos.x + size*0.5f, pos.y + size*0.5f);
    float radius = size * 0.45f;
    int numSegments = 30;

    float spinTime = 1.f; // seconds
    float currentTimeProgress = fmodf(ImGui::GetTime(), spinTime) / spinTime;
    float startAngle =  RADIANS_CIRCLE * currentTimeProgress;
    float endAngle = startAngle + (RADIANS_CIRCLE * 0.75);
    draw_list->PathArcTo(center, radius, startAngle, endAngle, numSegments);
    draw_list->PathStroke(color, 0, thickness);

    ImGui::Dummy(bb);
}