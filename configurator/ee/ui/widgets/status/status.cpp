#include "status.h"

#include "../icons/misc.h"
#include "spinner.h"

const ImU32 COLOR_SUCCESS = IM_COL32(0x00, 0xD0, 0x00, 0xFF);
const ImU32 COLOR_ERROR = IM_COL32(0xFF, 0x00, 0x00, 0xFF);
const ImU32 COLOR_WARNING = IM_COL32(0xFF, 0xFF, 0x00, 0xFF);

void PS2Plus::UI::StatusText(const char *label, StatusType status, ...) {
    va_list args;
    va_start(args, status);

    float iconSize = ImGui::GetFontSize();
    switch (status) {
        case StatusType_Success:
            PS2Plus::UI::Icons::Checkmark(iconSize, COLOR_SUCCESS);
            break;

        case StatusType_Error:
            PS2Plus::UI::Icons::Error(iconSize, COLOR_ERROR);
            break;

        case StatusType_Warning:
            PS2Plus::UI::Icons::Warning(iconSize, COLOR_WARNING);
            break;

        case StatusType_Loading:
            PS2Plus::UI::Spinner(iconSize, ImGui::GetColorU32(ImGuiCol_FrameBgActive), 4.f);
            break;

        default:
            ImGui::Dummy(ImVec2(iconSize, iconSize));
            break;
    }

    ImGui::SameLine();
    ImGui::TextWrappedV(label, args);
    va_end(args);
}