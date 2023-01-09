#include "icons.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

namespace PS2Plus::Components {
    void IconCheckmark(float size, ImU32 color) {
        size *= 0.85;
        ImVec2 bb(size, size);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        float thickness = ImMax(size / 5.0f, 1.0f);
        size -= thickness * 0.5f;
        pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

        float third = size / 3.0f;
        float bx = pos.x + third;
        float by = pos.y + size - third * 0.5f;
        draw_list->PathLineTo(ImVec2(bx - third, by - third));
        draw_list->PathLineTo(ImVec2(bx, by));
        draw_list->PathLineTo(ImVec2(bx + third * 2.0f, by - third * 2.0f));
        draw_list->PathStroke(color, 0, thickness);

        ImGui::Dummy(bb);
    }

    void IconError(float size, ImU32 color) {
        size *= 0.85;
        ImVec2 bb(size, size);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        float thickness = ImMax(size / 5.0f, 1.0f);
        size -= thickness * 0.5f;
        pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

        ImVec2 center = pos + ImVec2(size * 0.5f, size * 0.5f);
        float extent = size * 0.4f;
        draw_list->AddLine(ImVec2(center.x - extent, center.y - extent), ImVec2(center.x + extent, center.y + extent), color, thickness);
        draw_list->AddLine(ImVec2(center.x - extent, center.y + extent), ImVec2(center.x + extent, center.y - extent), color, thickness);

        ImGui::Dummy(bb);
    }

    void IconWarning(float size, ImU32 color) {
        size *= 0.85;
        ImVec2 bb(size, size);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        float thickness = ImMax(size / 10.0f, 1.0f);
        size -= thickness * 0.5f;
        pos += ImVec2(thickness * 0.25f, thickness * 0.25f);
        
        ImVec2 center = pos + ImVec2(size * 0.5f, size * 0.5f);
        float extent = size * 0.4f;
        draw_list->AddTriangleFilled(
            ImVec2(center.x - extent, center.y + extent), 
            ImVec2(center.x + extent, center.y + extent), 
            ImVec2(center.x, center.y - extent + 1), 
            color);
        draw_list->AddTriangle(
            ImVec2(center.x - extent, center.y + extent), 
            ImVec2(center.x + extent, center.y + extent), 
            ImVec2(center.x, center.y - extent + 1), 
            color, 
            thickness);

        ImGui::Dummy(bb);
    }

    void IconArrow(float size, ImU32 color, ImGuiDir dir) {
        size *= 0.85;
        ImVec2 bb(size, size);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        const float h = size;
        float r = h * 0.40f;
        ImVec2 center = pos + ImVec2(h * 0.50f, h * 0.50f);

        ImVec2 a, b, c;
        switch (dir)
        {
        case ImGuiDir_Up:
        case ImGuiDir_Down:
            if (dir == ImGuiDir_Up) r = -r;
            a = ImVec2(+0.000f, +0.750f) * r;
            b = ImVec2(-0.866f, -0.750f) * r;
            c = ImVec2(+0.866f, -0.750f) * r;
            break;
        case ImGuiDir_Left:
        case ImGuiDir_Right:
            if (dir == ImGuiDir_Left) r = -r;
            a = ImVec2(+0.750f, +0.000f) * r;
            b = ImVec2(-0.750f, +0.866f) * r;
            c = ImVec2(-0.750f, -0.866f) * r;
            break;
        case ImGuiDir_None:
        case ImGuiDir_COUNT:
            IM_ASSERT(0);
            break;
        }
        draw_list->AddTriangleFilled(center + a, center + b, center + c, color);

        ImGui::Dummy(bb);
    }
}