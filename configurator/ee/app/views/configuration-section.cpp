#include "app.h"
#include "app/views/configuration/export-dialog.h"
#include "app/views/configuration/import-dialog.h"
#include "app/views/configuration/options-dialog.h"
#include "app/views/configuration/reset-defaults-dialog.h"
#include "components/file-dialog.h"
#include "components/footer-command-menu.h"
#include "libps2plman.h"
#include "ui/drawing/drawing.h"
#include "ui/fonts/forkawesome.h"
#include "ui/fonts/playstation.h"

#include <shared/versions.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace PS2Plus::App::Views {
// Staging configuration object that will receive changes from the UI
PS2Plus::App::Configuration staging_config(VERSION_CONFIGURATION);

// Flag to tell the application to reload configuration on the next frame
bool is_configuration_stale = true;

const int kDigitalButtonsPerRow = 4;
const std::vector<std::pair<ps2plus_controller_digital_button, const char *>> kDigitalButtonLabels = {
    {DBTriangle, ICON_PLAYSTATION_TRIANGLE_BUTTON},
    {DBCircle, ICON_PLAYSTATION_CIRCLE_BUTTON},
    {DBCross, ICON_PLAYSTATION_CROSS_BUTTON},
    {DBSquare, ICON_PLAYSTATION_SQUARE_BUTTON},
    {DDUp, ICON_PLAYSTATION_DS3_DPAD_FULL_UP},
    {DDDown, ICON_PLAYSTATION_DS3_DPAD_FULL_DOWN},
    {DDLeft, ICON_PLAYSTATION_DS3_DPAD_FULL_LEFT},
    {DDRight, ICON_PLAYSTATION_DS3_DPAD_FULL_RIGHT},
    {DBL1, ICON_PLAYSTATION_L1_BUTTON},
    {DBR1, ICON_PLAYSTATION_R1_BUTTON},
    {DBL2, ICON_PLAYSTATION_L2_BUTTON},
    {DBR2, ICON_PLAYSTATION_R2_BUTTON},
    {DBL3, ICON_PLAYSTATION_L3_BUTTON},
    {DBR3, ICON_PLAYSTATION_R3_BUTTON},
    {DBStart, ICON_PLAYSTATION_START_BUTTON_LABEL},
    {DBSelect, ICON_PLAYSTATION_SELECT_BUTTON_LABEL},
};

// Reusable modal dialog components
PS2Plus::App::Views::Configuration::OptionsDialog options_dialog(staging_config);
PS2Plus::App::Views::Configuration::ExportDialog export_dialog(staging_config);
PS2Plus::App::Views::Configuration::ImportDialog import_dialog(staging_config);
PS2Plus::App::Views::Configuration::ResetDefaultsDialog reset_defaults_dialog(staging_config);

/**
 * Render a single combo box that can control the button remapping for a single digital button
 * @param button_id Digital button that this remapping control is for
 * @param p_button_remapping Pointer that contains the current mapping for `button_id` and will receive the new remapped value upon changes
 */
void CreateButtonRemappingComboBox(const char *combo_id, ps2plus_controller_digital_button button_id, uint8_t *p_button_remapping) {
  ps2plus_controller_digital_button current_remapped = (ps2plus_controller_digital_button)*p_button_remapping;
  bool is_remapped = (current_remapped != button_id);

  // Identify the index of the current remapped button value so it can be highlighted in the combo box
  uint8_t current_item_index = 0;
  for (size_t i = 0; i < kDigitalButtonLabels.size(); i++) {
    if (kDigitalButtonLabels[i].first == current_remapped) {
      current_item_index = i;
      break;
    }
  }

  // Show remapped buttons with a highlighted combo box style
  if (is_remapped) {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.48f, 0.29f, 0.48f, 0.54f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.98f, 0.59f, 0.98f, 0.40f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.98f, 0.59f, 0.98f, 0.67f));
  }

  if (ImGui::BeginCombo(combo_id, kDigitalButtonLabels[current_item_index].second, ImGuiComboFlags_NoArrowButton)) {
    for (int i = 0; i < kDigitalButtonLabels.size(); i++) {
      const auto button_label = kDigitalButtonLabels[i];
      const bool is_selected = (current_item_index == i);
      if (ImGui::Selectable(button_label.second, is_selected)) {
        // Update the button remapping pointer with this selected button ID
        *p_button_remapping = button_label.first;
      }

      // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  // Unset the combo box style for remapped buttons
  if (is_remapped) {
    ImGui::PopStyleColor(/* ImGuiCol_FrameBgActive */);
    ImGui::PopStyleColor(/* ImGuiCol_FrameBgHovered */);
    ImGui::PopStyleColor(/* ImGuiCol_FrameBg */);
  }
}

void CreateButtonRemappingTableCell(uint8_t **p_button_remappings, ps2plus_controller_digital_button button_id, const char *button_label) {
  // First column contains the button's icon and a right arrow
  ImGui::TableNextColumn();
  ImGui::AlignTextToFramePadding();
  ImGui::Text(button_label);
  ImGui::SameLine();
  ImGui::Text(ICON_FK_LONG_ARROW_RIGHT);

  // Second colunn contains the button's remapping combo box (button ID is pushed to the Dear ImGui stack to avoid conflicts)
  ImGui::TableNextColumn();
  ImGui::PushID((int)button_id);
  CreateButtonRemappingComboBox("##Remap", button_id, p_button_remappings[button_id]);
  ImGui::PopID();
}

void ButtonsConfiguration_Remapping() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  bool *p_enable_button_remapping = &staging_config.GetRawStructure().enable_button_remapping.boolean;

  // Setup pointers to each of the joystick axis range values
  uint8_t *p_button_remappings[NUM_DIGITAL_BUTTONS];
  for (size_t i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    p_button_remappings[i] = &staging_config.GetRawStructure().button_remapping[i].uint8;
  }

  ImGui::Checkbox("##enable_button_remapping", p_enable_button_remapping);
  ImGui::SameLine();
  ImGui::TextWrapped("Enable remapping of digital controller buttons");

  ImGui::BeginDisabled(!*p_enable_button_remapping);
  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2, 2));
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
  ImGui::Indent(28.f);

  // Create the remapping table with two columns for each button cell
  if (ImGui::BeginTable("Button Remapping", kDigitalButtonsPerRow * 2)) {
    // Setup each button's column width policies
    ImGui::PushID("ColumnSetup");
    for (int i = 0; i < kDigitalButtonsPerRow; i++) {
      ImGui::PushID(i);
      ImGui::TableSetupColumn("##OriginalButton", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("##RemappedButton", ImGuiTableColumnFlags_WidthStretch);
      ImGui::PopID();
    }
    ImGui::PopID();

    // Setup each button cell based on the "cells per row" property
    ImGui::PushID("Buttons");
    for (int i = 0; i < kDigitalButtonLabels.size(); i++) {
      // First cell in each row should move to the next row
      if (i % kDigitalButtonsPerRow == 0) {
        ImGui::TableNextRow();
      }

      // Fill out the columns for this button
      auto button_entry = kDigitalButtonLabels[i];
      CreateButtonRemappingTableCell(p_button_remappings, button_entry.first, button_entry.second);
    }
    ImGui::PopID();

    ImGui::EndTable();
  }
  ImGui::Unindent(28.f);
  ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
  ImGui::PopStyleVar(/* ImGuiStyleVar_CellPadding */);
  ImGui::EndDisabled();
}

void ButtonsConfiguration() {
  if (ImGui::TreeNodeEx("Buttons", ImGuiTreeNodeFlags_DefaultOpen)) {
    ButtonsConfiguration_Remapping();

    ImGui::TreePop();
    ImGui::Separator();
  }
}

void JoysticksConfiguration_Deadzones() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  uint8_t *p_joystick_deadzone_left = &staging_config.GetRawStructure().joystick_deadzone_left.uint8;
  uint8_t *p_joystick_deadzone_right = &staging_config.GetRawStructure().joystick_deadzone_right.uint8;
  static uint8_t value_min = 0, value_max = 127;

  if (ImGui::TreeNodeEx("Deadzones##Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::BeginGroup();
    {
      const float joystick_size = 90.f;
      const ImVec2 p = ImGui::GetCursorScreenPos();
      const ImVec2 center = ImVec2(p.x + joystick_size / 2, p.y + joystick_size / 2);
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      draw_controller_joystick(draw_list, center, joystick_size / 2, false, state.current_gamepad().pad.ljoy_h,
                               state.current_gamepad().pad.ljoy_v);

      if (*p_joystick_deadzone_left) {
        draw_list->AddCircleFilled(center, joystick_size / 2 * (*p_joystick_deadzone_left / 128.f), IM_COL32(0xC0, 0x33, 0x33, 0x33));
      }

      ImGui::Dummy(ImVec2(joystick_size, joystick_size));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      const float joystick_size = 90.f;
      const ImVec2 p = ImGui::GetCursorScreenPos();
      const ImVec2 center = ImVec2(p.x + joystick_size / 2, p.y + joystick_size / 2);
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      draw_controller_joystick(draw_list, center, joystick_size / 2, false, state.current_gamepad().pad.rjoy_h,
                               state.current_gamepad().pad.rjoy_v);

      if (*p_joystick_deadzone_right) {
        draw_list->AddCircleFilled(center, joystick_size / 2 * (*p_joystick_deadzone_right / 128.f), IM_COL32(0xC0, 0x33, 0x33, 0x33));
      }

      ImGui::Dummy(ImVec2(joystick_size, joystick_size));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::PushItemWidth(50.f);
      ImGui::DragScalar("Left", ImGuiDataType_U8, p_joystick_deadzone_left, 1.0, &value_min, &value_max);
      ImGui::DragScalar("Right", ImGuiDataType_U8, p_joystick_deadzone_right, 1.0, &value_min, &value_max);
      ImGui::PopItemWidth();
    }
    ImGui::EndGroup();

    ImGui::TreePop();
    ImGui::Separator();
  }
}

void JoysticksConfiguration_DigitalMode() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  uint8_t *p_joystick_digital_threshold_left = &staging_config.GetRawStructure().joystick_digital_threshold_left.uint8;
  uint8_t *p_joystick_digital_threshold_right = &staging_config.GetRawStructure().joystick_digital_threshold_right.uint8;
  int joystick_digital_enable_flags = ((staging_config.GetRawStructure().joystick_digital_enable_left.boolean ? 0b01 : 0b00) |
                                       (staging_config.GetRawStructure().joystick_digital_enable_right.boolean ? 0b10 : 0b00));

  static uint8_t threshold_min = 0, threshold_max = 0x7F;

  // We have to manually track changes to the left/right enable booleans, since we're using
  // them indirectly with ImGui as checkbox flags (to enable a mixed-state between left/right).
  bool enabled_changed = false;

  if (ImGui::TreeNodeEx("Digital Mode##Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {

    enabled_changed =
        ImGui::CheckboxFlags("Enable using the joysticks in digital mode", &joystick_digital_enable_flags, 0b11) || enabled_changed;
    ImGui::Indent();
    {
      enabled_changed =
          ImGui::CheckboxFlags("##Joysticks/joystick_digital_enable_left", &joystick_digital_enable_flags, 0b01) || enabled_changed;
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::TextWrapped("Left joystick simulates the D-pad");
        ImGui::BeginDisabled(!(joystick_digital_enable_flags & 0b01));
        {
          ImGui::Text("Threshold: ");
          ImGui::SameLine();
          ImGui::PushItemWidth(50.f);
          ImGui::DragScalar("##Joysticks/joystick_digital_threshold_left", ImGuiDataType_U8, p_joystick_digital_threshold_left, 1.0,
                            &threshold_min, &threshold_max);
          ImGui::PopItemWidth();
        }
        ImGui::EndDisabled();
      }
      ImGui::EndGroup();

      enabled_changed =
          ImGui::CheckboxFlags("##Joysticks/joystick_digital_enable_right", &joystick_digital_enable_flags, 0b10) || enabled_changed;
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::TextWrapped("Right joystick simulates the face buttons");
        ImGui::BeginDisabled(!(joystick_digital_enable_flags & 0b10));
        {
          ImGui::Text("Threshold: ");
          ImGui::SameLine();
          ImGui::PushItemWidth(50.f);
          ImGui::DragScalar("##Joysticks/joystick_digital_threshold_right", ImGuiDataType_U8, p_joystick_digital_threshold_right, 1.0,
                            &threshold_min, &threshold_max);
          ImGui::PopItemWidth();
        }
        ImGui::EndDisabled();
      }
      ImGui::EndGroup();
    }
    ImGui::Unindent();

    ImGui::TreePop();
    ImGui::Separator();
  }

  if (enabled_changed) {
    staging_config.GetRawStructure().joystick_digital_enable_left.boolean = joystick_digital_enable_flags & 0b01;
    staging_config.GetRawStructure().joystick_digital_enable_right.boolean = joystick_digital_enable_flags & 0b10;
  }
}

void JoysticksConfiguration_AxisRangeRemapping() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  bool *p_enable_joystick_axis_range_remapping = &staging_config.GetRawStructure().enable_joystick_axis_range_remapping.boolean;

  // Setup pointers to each of the joystick axis range values
  uint8_t *p_joystick_axis_range_remappings[NUM_JOYSTICK_AXIS_RANGES];
  for (size_t i = 0; i < NUM_JOYSTICK_AXIS_RANGES; i++) {
    p_joystick_axis_range_remappings[i] = &staging_config.GetRawStructure().joystick_axis_range_remapping[i].uint8;
  }

  if (ImGui::TreeNodeEx("Axis Range Remapping##Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {
    static bool is_calibrating = false;

    ImGui::Checkbox("##enable_button_remapping", p_enable_joystick_axis_range_remapping);
    ImGui::SameLine();
    ImGui::TextWrapped("Enable joystick axis range remapping");

    ImGui::BeginDisabled(!(*p_enable_joystick_axis_range_remapping));
    ImGui::BeginGroup();
    {
      const ImVec2 p = ImGui::GetCursorScreenPos();
      const float joystick_size = 90.f;
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      draw_controller_joystick(draw_list, ImVec2(p.x + joystick_size / 2, p.y + joystick_size / 2), joystick_size / 2, is_calibrating,
                               *p_joystick_axis_range_remappings[JSAxisRange_LeftXCenter],
                               *p_joystick_axis_range_remappings[JSAxisRange_LeftYCenter]);

      draw_list->AddRectFilled(
          ImVec2(p.x + joystick_size / 2 - joystick_size / 2 * (1 - (float)*p_joystick_axis_range_remappings[JSAxisRange_LeftXMin] / 128),
                 p.y + joystick_size / 2 - joystick_size / 2 * (1 - (float)*p_joystick_axis_range_remappings[JSAxisRange_LeftYMin] / 128)),
          ImVec2(p.x + joystick_size / 2 + joystick_size / 2 * ((float)*p_joystick_axis_range_remappings[JSAxisRange_LeftXMax] / 128 - 1),
                 p.y + joystick_size / 2 + joystick_size / 2 * ((float)*p_joystick_axis_range_remappings[JSAxisRange_LeftYMax] / 128 - 1)),
          IM_COL32(0x55, 0x55, 0xAA, 0x60), 5.0f);

      if (is_calibrating) {
        draw_list->AddCircleFilled(ImVec2(p.x + joystick_size * (state.current_gamepad().pad.ljoy_h / 255.f),
                                          p.y + joystick_size * (state.current_gamepad().pad.ljoy_v / 255.f)),
                                   2.0f, IM_COL32(0x55, 0x55, 0xAA, 0xC0));
      }

      ImGui::Dummy(ImVec2(joystick_size, joystick_size));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      const ImVec2 p = ImGui::GetCursorScreenPos();
      const float joystick_size = 90.f;
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      draw_controller_joystick(draw_list, ImVec2(p.x + joystick_size / 2, p.y + joystick_size / 2), joystick_size / 2, is_calibrating,
                               *p_joystick_axis_range_remappings[JSAxisRange_RightXCenter],
                               *p_joystick_axis_range_remappings[JSAxisRange_RightYCenter]);

      draw_list->AddRectFilled(
          ImVec2(p.x + joystick_size / 2 - joystick_size / 2 * (1 - (float)*p_joystick_axis_range_remappings[JSAxisRange_RightXMin] / 128),
                 p.y + joystick_size / 2 - joystick_size / 2 * (1 - (float)*p_joystick_axis_range_remappings[JSAxisRange_RightYMin] / 128)),
          ImVec2(p.x + joystick_size / 2 + joystick_size / 2 * ((float)*p_joystick_axis_range_remappings[JSAxisRange_RightXMax] / 128 - 1),
                 p.y + joystick_size / 2 + joystick_size / 2 * ((float)*p_joystick_axis_range_remappings[JSAxisRange_RightYMax] / 128 - 1)),
          IM_COL32(0x55, 0x55, 0xAA, 0x60), 5.0f);

      if (is_calibrating) {
        draw_list->AddCircleFilled(ImVec2(p.x + joystick_size * (state.current_gamepad().pad.rjoy_h / 255.f),
                                          p.y + joystick_size * (state.current_gamepad().pad.rjoy_v / 255.f)),
                                   2.0f, IM_COL32(0x55, 0x55, 0xAA, 0xC0));
      }

      ImGui::Dummy(ImVec2(joystick_size, joystick_size));
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(is_calibrating);
      if (ImGui::Button("Calibrate") && !is_calibrating) {
        // ps2plman_disable_enable_configuration(false, NULL);
        is_calibrating = true;
        BeginCaptureGamepad();

        for (int i = 0; i < NUM_JOYSTICK_AXIS_RANGES; i++) {
          *p_joystick_axis_range_remappings[i] = 127;
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset##JoystickAxisRangeMapping")) {
        for (int i = 0; i < NUM_JOYSTICK_AXIS_RANGES / 3; i++) {
          *p_joystick_axis_range_remappings[i * 3 + 0] = 0;
          *p_joystick_axis_range_remappings[i * 3 + 1] = 127;
          *p_joystick_axis_range_remappings[i * 3 + 2] = 255;
        }
      }
      ImGui::EndDisabled();

      if (is_calibrating) {
        ImGui::TextColored(ICON_PLAYSTATION_COLOR_SQUARE, ICON_PLAYSTATION_SQUARE_BUTTON);
        ImGui::SameLine();
        ImGui::Text("Update centers");

        ImGui::TextColored(ICON_PLAYSTATION_COLOR_CIRCLE, ICON_PLAYSTATION_CIRCLE_BUTTON);
        ImGui::SameLine();
        ImGui::Text("Finish calibration");
      }

      if (is_calibrating && state.current_gamepad().IsButtonPressed(PAD_SQUARE)) {
        *p_joystick_axis_range_remappings[JSAxisRange_LeftXCenter] = state.current_gamepad().pad.ljoy_h;
        *p_joystick_axis_range_remappings[JSAxisRange_LeftYCenter] = state.current_gamepad().pad.ljoy_v;
        *p_joystick_axis_range_remappings[JSAxisRange_RightXCenter] = state.current_gamepad().pad.rjoy_h;
        *p_joystick_axis_range_remappings[JSAxisRange_RightYCenter] = state.current_gamepad().pad.rjoy_v;
      }

      if (is_calibrating && state.current_gamepad().IsButtonPressed(PAD_CIRCLE)) {
        is_calibrating = false;
        EndCaptureGamepad();
        // ps2plman_disable_enable_configuration(true, NULL);
      }

      if (is_calibrating) {
        if (state.current_gamepad().pad.ljoy_h < *p_joystick_axis_range_remappings[JSAxisRange_LeftXMin])
          *p_joystick_axis_range_remappings[JSAxisRange_LeftXMin] = state.current_gamepad().pad.ljoy_h;
        if (state.current_gamepad().pad.ljoy_h > *p_joystick_axis_range_remappings[JSAxisRange_LeftXMax])
          *p_joystick_axis_range_remappings[JSAxisRange_LeftXMax] = state.current_gamepad().pad.ljoy_h;
        if (state.current_gamepad().pad.ljoy_v < *p_joystick_axis_range_remappings[JSAxisRange_LeftYMin])
          *p_joystick_axis_range_remappings[JSAxisRange_LeftYMin] = state.current_gamepad().pad.ljoy_v;
        if (state.current_gamepad().pad.ljoy_v > *p_joystick_axis_range_remappings[JSAxisRange_LeftYMax])
          *p_joystick_axis_range_remappings[JSAxisRange_LeftYMax] = state.current_gamepad().pad.ljoy_v;

        if (state.current_gamepad().pad.rjoy_h < *p_joystick_axis_range_remappings[JSAxisRange_RightXMin])
          *p_joystick_axis_range_remappings[JSAxisRange_RightXMin] = state.current_gamepad().pad.rjoy_h;
        if (state.current_gamepad().pad.rjoy_h > *p_joystick_axis_range_remappings[JSAxisRange_RightXMax])
          *p_joystick_axis_range_remappings[JSAxisRange_RightXMax] = state.current_gamepad().pad.rjoy_h;
        if (state.current_gamepad().pad.rjoy_v < *p_joystick_axis_range_remappings[JSAxisRange_RightYMin])
          *p_joystick_axis_range_remappings[JSAxisRange_RightYMin] = state.current_gamepad().pad.rjoy_v;
        if (state.current_gamepad().pad.rjoy_v > *p_joystick_axis_range_remappings[JSAxisRange_RightYMax])
          *p_joystick_axis_range_remappings[JSAxisRange_RightYMax] = state.current_gamepad().pad.rjoy_v;
      }
    }
    ImGui::EndGroup();

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2, 2));
    ImGui::BeginDisabled();
    if (ImGui::BeginTable("Joystick Axis Range Remapping", 8)) {
      uint8_t first_min = 0, first_max = 127, second_min = 128, second_max = 255;

      ImGui::TableSetupColumn("##LX-Label", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("##LX-Value", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("##LY-Label", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("##LY-Value", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("##RX-Label", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("##RX-Value", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("##RY-Label", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("##RY-Value", ImGuiTableColumnFlags_WidthStretch);

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("LX-");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##LX-", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_LeftXMin], 1.0F, &first_min, &first_max);
      ImGui::TableNextColumn();
      ImGui::Text("LY-");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##LY-", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_LeftYMin], 1.0F, &first_min, &first_max);
      ImGui::TableNextColumn();
      ImGui::Text("RX-");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##RX-", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_RightXMin], 1.0F, &first_min, &first_max);
      ImGui::TableNextColumn();
      ImGui::Text("RY-");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##RY-", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_RightYMin], 1.0F, &first_min, &first_max);

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("LX");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##LX", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_LeftXCenter], 1.0F,
                        p_joystick_axis_range_remappings[JSAxisRange_LeftXMin], p_joystick_axis_range_remappings[JSAxisRange_LeftXMax]);
      ImGui::TableNextColumn();
      ImGui::Text("LY");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##LY", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_LeftYCenter], 1.0F,
                        p_joystick_axis_range_remappings[JSAxisRange_LeftYMin], p_joystick_axis_range_remappings[JSAxisRange_LeftYMax]);
      ImGui::TableNextColumn();
      ImGui::Text("RX");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##RX", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_RightXCenter], 1.0F,
                        p_joystick_axis_range_remappings[JSAxisRange_RightXMin], p_joystick_axis_range_remappings[JSAxisRange_RightXMax]);
      ImGui::TableNextColumn();
      ImGui::Text("RY");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##RY", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_RightYCenter], 1.0F,
                        p_joystick_axis_range_remappings[JSAxisRange_RightYMin], p_joystick_axis_range_remappings[JSAxisRange_RightYMax]);

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("LX+");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##LX+", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_LeftXMax], 1.0F, &second_min, &second_max);
      ImGui::TableNextColumn();
      ImGui::Text("LY+");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##LY+", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_LeftYMax], 1.0F, &second_min, &second_max);
      ImGui::TableNextColumn();
      ImGui::Text("RX+");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##RX+", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_RightXMax], 1.0F, &second_min, &second_max);
      ImGui::TableNextColumn();
      ImGui::Text("RY+");
      ImGui::TableNextColumn();
      ImGui::DragScalar("##RY+", ImGuiDataType_U8, p_joystick_axis_range_remappings[JSAxisRange_RightYMax], 1.0F, &second_min, &second_max);

      ImGui::EndTable();
    }
    ImGui::EndDisabled();
    ImGui::PopStyleVar();
    ImGui::EndDisabled();

    ImGui::TreePop();
    ImGui::Separator();
  }
}

void JoysticksConfiguration() {
  if (ImGui::TreeNodeEx("Joysticks", ImGuiTreeNodeFlags_DefaultOpen)) {
    JoysticksConfiguration_Deadzones();
    JoysticksConfiguration_DigitalMode();
    JoysticksConfiguration_AxisRangeRemapping();

    ImGui::TreePop();
  }
}

void ReloadConfiguration() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  printf("Reloading configuration\n");
  // TODO: Create and call a "ReloadConfig" method on the current controller
  staging_config = PS2Plus::App::Configuration(VERSION_CONFIGURATION, &state.current_controller()->configuration().GetRawStructure());
  is_configuration_stale = false;
}

void PersistConfiguration() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  printf("Saving configuration\n");

  // Persist changes to the controller
  auto new_configs = state.current_controller()->configuration().GetDifferences(staging_config);
  printf("Num changed configs: %d\n", new_configs.size());
  if (!new_configs.empty()) {
    // TODO: Create and call a "PersisConfig" method on the current controller
    PS2Plus::Gamepad::Stop();
    for (auto const& config_pair : new_configs) {
      printf("Updating configuration %d\n", config_pair.first);
      ps2plman_set_configuration(config_pair.first, config_pair.second);
    }
    PS2Plus::Gamepad::Start();
  }

  is_configuration_stale = true;
}

void FooterMenu(ImVec2 child_window_pos, ImVec2 child_window_size) {
  PS2Plus::App::State& state = PS2Plus::App::GetState();

  PS2Plus::Components::BeginFooterCommandMenu("ConfigurationFooterMenu", PS2Plus::Components::kFooterCommandMenuEnabledWithGlobalNav);
  {
    if (PS2Plus::Components::FooterCommand("Save", ICON_PLAYSTATION_START_BUTTON, ICON_PLAYSTATION_COLOR_GREEN, ImGuiKey_GamepadStart)) {
    }

    if (PS2Plus::Components::FooterCommand("Options", ICON_PLAYSTATION_SQUARE_BUTTON, ICON_PLAYSTATION_COLOR_SQUARE,
                                           ImGuiKey_GamepadFaceLeft)) {
      if (!options_dialog.IsOpen()) {
        options_dialog.Open();
      }
    }
  }
  PS2Plus::Components::EndFooterCommandMenu();

  if (auto options_result = options_dialog.Render()) {
    switch (options_result.value()) {
      case PS2Plus::App::Views::Configuration::kOptionsResultResetDefaults:
        reset_defaults_dialog.Open();
        break;

      case PS2Plus::App::Views::Configuration::kOptionsResultImport:
        import_dialog.Open();
        break;

      case PS2Plus::App::Views::Configuration::kOptionsResultExport:
        export_dialog.Open();
        break;
    }
  }

  import_dialog.Render();
  export_dialog.Render();
  reset_defaults_dialog.Render();

  // PersistConfiguration();
}

void VersionMismatchView() {
  const char *label = ("    Configuration versions do not match between    \n"
                       "     the configurator and the PS2+ controller.     \n"
                       "                                                   \n"
                       "Please upgrade both the configurator and the latest\n"
                       "  version to enable modifying PS2+ configuration.  ");
  ImVec2 label_size = ImGui::CalcTextSize(label);
  ImGui::SetCursorPos((ImGui::GetWindowSize() - label_size) * 0.5f);
  ImGui::Text(label);
}

void ConfigurationSection() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();

  if (!state.current_controller() || !state.current_controller()->connected()) {
    NoPS2PlusConnectedMessage();
    return;
  }

  // if (state.current_controller()->versions().configuration != staging_config.version()) {
  //   VersionMismatchView();
  //   return;
  // }

  if (is_configuration_stale) {
    ReloadConfiguration();
  }

  // Track the position and size of child window for use in the footer's option overlay menu
  ImVec2 child_window_pos;
  ImVec2 child_window_size;

  ImGui::BeginChild("ConfigurationChildSection", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NavFlattened);
  {
    ButtonsConfiguration();
    JoysticksConfiguration();

    child_window_pos = ImGui::GetWindowPos();
    child_window_size = ImGui::GetWindowSize();
  }
  ImGui::EndChild();

  FooterMenu(child_window_pos, child_window_size);
}
} // namespace PS2Plus::App::Views