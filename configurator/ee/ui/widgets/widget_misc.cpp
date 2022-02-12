#include "widget.h"

void ImGui::Widgets::WindowOverlay(float alpha) {
    ImGui::Widgets::WindowOverlay(IM_COL32(0, 0, 0, 0xFF * alpha));
}

void ImGui::Widgets::WindowOverlay(ImU32 color) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    drawList->AddRectFilled(windowPos, 
        ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
        color);
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
void ImGui::Widgets::HelpMarker(const char* desc)
{
    ImGui::PushID(desc);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0));
    ImGui::SmallButton("?");
    ImGui::PopStyleVar();
    ImGui::PopID();
    if ((ImGui::GetIO().MouseDrawCursor && ImGui::IsItemHovered()) || (!ImGui::GetIO().MouseDrawCursor && ImGui::IsItemFocused())) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}