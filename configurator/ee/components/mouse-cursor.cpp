#include "mouse-cursor.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace PS2Plus::Components {
    void DrawCustomPointerCursor() {
        ImVec2 center = ImGui::GetMousePos();
        ImDrawList *drawList = ImGui::GetForegroundDrawList();
        drawList->AddCircleFilled(center, 12, IM_COL32(255, 255, 255, 32), 0);
        drawList->AddCircle(center, 12, IM_COL32(255, 255, 255, 128), 0, 2.0f);

        float angle_offset = ImGui::GetTime() / 6.f * 2 * M_PI;
        const float arc_size = M_PI / 2;
        const float arc_gap = arc_size * 0.2;
        for (int i = 0; i < 4; i++) {
            float angle_start = angle_offset + (i * arc_size);
            float angle_end = angle_start + arc_size - arc_gap;
            drawList->PathArcTo(center, 6.f, angle_start, angle_end);

            // Smoothly animate opacity between 80 and 100 (out of 255)
            float opacity = 90 + sinf(angle_offset * 4) * 20;
            drawList->PathStroke(IM_COL32(255, 255, 255, (uint8_t)opacity), 0, 2.0f);
        }
    }

    void MouseCursor() {
        ImGuiIO& io = ImGui::GetIO();

        if (!io.MouseDrawCursor) {
            return;
        }

        switch (ImGui::GetMouseCursor()) {
            case ImGuiMouseCursor_None:
            case ImGuiMouseCursor_Arrow:
                DrawCustomPointerCursor();
                ImGui::SetMouseCursor(ImGuiMouseCursor_None);
                break;
        }
    }
}