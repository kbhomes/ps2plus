#include "status.h"

#include "icons.h"
#include "spinner.h"

#include <imgui/imgui_internal.h>

namespace PS2Plus::Components {
    const ImU32 COLOR_SUCCESS = IM_COL32(0x00, 0xD0, 0x00, 0xFF);
    const ImU32 COLOR_ERROR = IM_COL32(0xFF, 0x00, 0x00, 0xFF);
    const ImU32 COLOR_WARNING = IM_COL32(0xFF, 0xFF, 0x00, 0xFF);

    void StatusText(const char *label, StatusType status, ...) {
        va_list args;
        va_start(args, status);

        float iconSize = ImGui::GetFontSize();
        switch (status) {
            case kStatusSuccess:
                IconCheckmark(iconSize, COLOR_SUCCESS);
                break;

            case kStatusError:
                IconError(iconSize, COLOR_ERROR);
                break;

            case kStatusWarning:
                IconWarning(iconSize, COLOR_WARNING);
                break;

            case kStatusLoading:
                Spinner(iconSize, ImGui::GetColorU32(ImGuiCol_FrameBgActive), 4.f);
                break;

            default:
                ImGui::Dummy(ImVec2(iconSize, iconSize));
                break;
        }

        ImGui::SameLine();
        ImGui::TextWrappedV(label, args);
        va_end(args);
    }

    void StatusTextCentered(const char *label, StatusType status, ...) {
        va_list args;
        va_start(args, status);
        

        const char *formatted_text, *formatted_ext_end;
        ImFormatStringToTempBufferV(&formatted_text, &formatted_ext_end, label, args);
        float text_width = ImGui::CalcTextSize(formatted_text, formatted_ext_end).x;

        float icon_size = ImGui::GetFontSize();
        float total_width = icon_size + ImGui::GetStyle().ItemSpacing.x + text_width;
        
        float distance = (ImGui::GetContentRegionAvail().x - total_width) / 2;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
        StatusText(label, status, args);
        va_end(args);
    }
}