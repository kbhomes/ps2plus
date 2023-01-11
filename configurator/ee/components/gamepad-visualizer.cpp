#include "gamepad-visualizer.h"
#include "ui/fonts/playstation.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include <math.h>

namespace PS2Plus::Components {
    void GamepadVisualizer(const char *id, const PS2Plus::Gamepad::PadStatus &pad, const ImVec2& size, bool disabled) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 aspect_size = size;

        // Find the largest 7x4 rectangle in this bounding box
        if (size.x / size.y > (7.f/4.f)) {
            // Wider than it is tall; shrink and center horizontally
            float aspect_x = (size.y / 4.f * 7.f);
            float overflow_x = size.x - aspect_x;
            p.x += overflow_x / 2;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (overflow_x / 2));
            aspect_size.x = aspect_x;
        } else if (size.x / size.y < (7.f/4.f)) {
            // Taller than it is wide, shrink and center vertically
            float aspect_y = (size.x / 7.f * 4.f);
            float overflow_y = size.y - aspect_y;
            p.y += overflow_y / 2;
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (overflow_y / 2));
            aspect_size.y = aspect_y;
        }

        float tile_x = aspect_size.x / 35;
        float tile_y = aspect_size.y / 20;

        ImVec4 disabled_color = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
        disabled_color.w = 0.25;

        if (disabled) {
            ImGui::PushStyleColor(ImGuiCol_Text, disabled_color);
        }

        ImGui::PushFont(PS2Plus::Graphics::GetFontPlayStationLarge());
        const ImVec2 local_pos = ImGui::GetCursorPos() - ImVec2(ImGui::GetFontSize() / 2, ImGui::GetFontSize() / 2);

        // Get all button/joystick values
        bool is_pad_up = disabled ? false : pad.IsButtonDown(PAD_UP);
        bool is_pad_down = disabled ? false : pad.IsButtonDown(PAD_DOWN);
        bool is_pad_left = disabled ? false : pad.IsButtonDown(PAD_LEFT);
        bool is_pad_right = disabled ? false : pad.IsButtonDown(PAD_RIGHT);
        bool is_pad_triangle = disabled ? false : pad.IsButtonDown(PAD_TRIANGLE);
        bool is_pad_cross = disabled ? false : pad.IsButtonDown(PAD_CROSS);
        bool is_pad_square = disabled ? false : pad.IsButtonDown(PAD_SQUARE);
        bool is_pad_circle = disabled ? false : pad.IsButtonDown(PAD_CIRCLE);
        bool is_pad_l1 = disabled ? false : pad.IsButtonDown(PAD_L1);
        bool is_pad_l2 = disabled ? false : pad.IsButtonDown(PAD_L2);
        bool is_pad_l3 = disabled ? false : pad.IsButtonDown(PAD_L3);
        bool is_pad_r1 = disabled ? false : pad.IsButtonDown(PAD_R1);
        bool is_pad_r2 = disabled ? false : pad.IsButtonDown(PAD_R2);
        bool is_pad_r3 = disabled ? false : pad.IsButtonDown(PAD_R3);
        bool is_pad_start = disabled ? false : pad.IsButtonDown(PAD_START);
        bool is_pad_select = disabled ? false : pad.IsButtonDown(PAD_SELECT);
        uint8_t pad_js_left_x = disabled ? 0x7F : pad.GetJoystickAxisRaw(JSLeftX);
        uint8_t pad_js_left_y = disabled ? 0x7F : pad.GetJoystickAxisRaw(JSLeftY);
        uint8_t pad_js_right_x = disabled ? 0x7F : pad.GetJoystickAxisRaw(JSRightX);
        uint8_t pad_js_right_y = disabled ? 0x7F : pad.GetJoystickAxisRaw(JSRightY);

        // Draw the directional buttons
        ImVec2 dpad_center = local_pos + ImVec2(tile_x * 8.5, tile_y * 8.5); 
        ImGui::SetCursorPos(dpad_center + ImVec2(0, tile_y * -1.5)); ImGui::Text(is_pad_up ? ICON_PLAYSTATION_DS3_DPAD_UP_ALT : ICON_PLAYSTATION_DS3_DPAD_UP);
        ImGui::SetCursorPos(dpad_center + ImVec2(0, tile_y * 1.5)); ImGui::Text(is_pad_down ? ICON_PLAYSTATION_DS3_DPAD_DOWN_ALT : ICON_PLAYSTATION_DS3_DPAD_DOWN);
        ImGui::SetCursorPos(dpad_center + ImVec2(tile_x * -1.5,0)); ImGui::Text(is_pad_left ? ICON_PLAYSTATION_DS3_DPAD_LEFT_ALT : ICON_PLAYSTATION_DS3_DPAD_LEFT);
        ImGui::SetCursorPos(dpad_center + ImVec2(tile_x * 1.5, 0)); ImGui::Text(is_pad_right ? ICON_PLAYSTATION_DS3_DPAD_RIGHT_ALT : ICON_PLAYSTATION_DS3_DPAD_RIGHT);

        // Draw the face buttons
        ImVec2 face_center = local_pos + ImVec2(tile_x * 26.5, tile_y * 8.5); 
        ImGui::SetCursorPos(face_center + ImVec2(0, tile_y * -2)); 
        ImGui::TextColored(
            disabled ? disabled_color : ICON_PLAYSTATION_COLOR_TRIANGLE, 
            is_pad_triangle ? ICON_PLAYSTATION_TRIANGLE_BUTTON_ALT : ICON_PLAYSTATION_TRIANGLE_BUTTON);
        ImGui::SetCursorPos(face_center + ImVec2(0, tile_y * 2)); 
        ImGui::TextColored(
            disabled ? disabled_color : ICON_PLAYSTATION_COLOR_CROSS, 
            is_pad_cross ? ICON_PLAYSTATION_CROSS_BUTTON_ALT : ICON_PLAYSTATION_CROSS_BUTTON);
        ImGui::SetCursorPos(face_center + ImVec2(tile_x * -2, 0)); 
        ImGui::TextColored(
            disabled ? disabled_color : ICON_PLAYSTATION_COLOR_SQUARE, 
            is_pad_square ? ICON_PLAYSTATION_SQUARE_BUTTON_ALT : ICON_PLAYSTATION_SQUARE_BUTTON);
        ImGui::SetCursorPos(face_center + ImVec2(tile_x * 2, 0)); 
        ImGui::TextColored(
            disabled ? disabled_color : ICON_PLAYSTATION_COLOR_CIRCLE, 
            is_pad_circle ? ICON_PLAYSTATION_CIRCLE_BUTTON_ALT : ICON_PLAYSTATION_CIRCLE_BUTTON);

        // Draw the L1/L2 buttons
        ImVec2 left_shoulder_center = local_pos + ImVec2(tile_x * 3.5, tile_y * 4.5); 
        ImGui::SetCursorPos(left_shoulder_center + ImVec2(0, tile_y * 1)); ImGui::Text(is_pad_l1 ? ICON_PLAYSTATION_L1_BUTTON_ALT : ICON_PLAYSTATION_L1_BUTTON);
        ImGui::SetCursorPos(left_shoulder_center + ImVec2(0, tile_y * -1)); ImGui::Text(is_pad_l2 ? ICON_PLAYSTATION_L2_BUTTON_ALT : ICON_PLAYSTATION_L2_BUTTON);

        // Draw the R1/R2 buttons
        ImVec2 right_shoulder_center = local_pos + ImVec2(tile_x * 31.5, tile_y * 4.5); 
        ImGui::SetCursorPos(right_shoulder_center + ImVec2(0, tile_y * 1)); ImGui::Text(is_pad_r1 ? ICON_PLAYSTATION_R1_BUTTON_ALT : ICON_PLAYSTATION_R1_BUTTON);
        ImGui::SetCursorPos(right_shoulder_center + ImVec2(0, tile_y * -1)); ImGui::Text(is_pad_r2 ? ICON_PLAYSTATION_R2_BUTTON_ALT : ICON_PLAYSTATION_R2_BUTTON);

        // Draw the start/select buttons
        ImVec2 start_select_center = local_pos + ImVec2(tile_x * 35 * 0.5f, tile_y * 8.5); 
        ImGui::SetCursorPos(start_select_center + ImVec2(tile_x * 2.5, 0)); ImGui::Text(is_pad_start ? ICON_PLAYSTATION_START_BUTTON_LABEL_ALT : ICON_PLAYSTATION_START_BUTTON_LABEL);
        ImGui::SetCursorPos(start_select_center + ImVec2(tile_x * -2.5, 0)); ImGui::Text(is_pad_select ? ICON_PLAYSTATION_SELECT_BUTTON_LABEL_ALT : ICON_PLAYSTATION_SELECT_BUTTON_LABEL);

        // Draw the left/right joysticks
        ImVec2 joystick_left_center = ImVec2(p.x + aspect_size.x*5/13, p.y + aspect_size.y*0.75);
        ImVec2 joystick_right_center = ImVec2(p.x + aspect_size.x*8/13, p.y + aspect_size.y*0.75);
        draw_controller_joystick(draw_list, joystick_left_center, aspect_size.x / 15, is_pad_l3, pad_js_left_x, pad_js_left_y, true, disabled ? ImGui::ColorConvertFloat4ToU32(disabled_color) : IM_COL32_WHITE, disabled);
        draw_controller_joystick(draw_list, joystick_right_center, aspect_size.x / 15, is_pad_r3, pad_js_right_x, pad_js_right_y, true, disabled ? ImGui::ColorConvertFloat4ToU32(disabled_color) : IM_COL32_WHITE, disabled);

        ImGui::SetCursorPos(local_pos);
        ImGui::InvisibleButton(id, size);
        ImGui::PopFont(/* FontPlayStationLarge */);

        if (disabled) {
            ImGui::PopStyleColor(/* ImGuiCol_Text */);
        }
    }
}