#include "loading-progress.h"

#include <math.h>
#include <cstring>
#include <cstdio>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

void PS2Plus::UI::LoadingProgress(const char *label, float progress, const char *format) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = ImGui::CalcItemWidth();

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));;

    ImGui::ItemSize(frame_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(frame_bb, id, &frame_bb, 0))
        return;

    // Default format string when passing NULL
    if (format == NULL)
        format = "%.1f%%";

    // Draw frame
    const ImU32 frame_col = ImGui::GetColorU32(ImGuiCol_FrameBg);
    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Render progress
    float progress_padding = 2.f;
    float max_width = frame_bb.GetWidth() - progress_padding*2;
    float current_width = max_width * progress;
    window->DrawList->AddRectFilled(
        ImVec2(frame_bb.Min.x + progress_padding, frame_bb.Min.y + progress_padding), 
        ImVec2(frame_bb.Min.x + progress_padding + current_width, frame_bb.Max.y - progress_padding), 
        ImGui::GetColorU32(ImGuiCol_SliderGrab),
        style.GrabRounding
    );

    // Display value using user-provided display format
    char value_buf[128];
    int label_len_original = strnlen(label, sizeof(value_buf) - 1);
    const char *label_end_display = ImGui::FindRenderedTextEnd(label, label + label_len_original);
    int label_len_display = label_end_display - label;
    strncpy(value_buf, label, label_len_display);
    snprintf(value_buf + label_len_display, sizeof(value_buf) - label_len_display - 1, format, progress * 100.f);
    int value_buf_len = strnlen(value_buf, sizeof(value_buf));

    ImGui::RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf + value_buf_len, NULL, ImVec2(0.5f, 0.5f));
}