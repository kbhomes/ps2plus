#include "status.h"

#include "icons.h"
#include "spinner.h"

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
}