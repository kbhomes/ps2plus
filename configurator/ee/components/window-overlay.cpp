#include "window-overlay.h"

namespace PS2Plus::Components {
    void WindowOverlay(float alpha) {
        WindowOverlay(IM_COL32(0, 0, 0, 0xFF * alpha));
    }

    void WindowOverlay(ImU32 color) {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        drawList->AddRectFilled(windowPos, 
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            color);
    }
}