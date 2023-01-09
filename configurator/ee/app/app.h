#pragma once

#include "../state.h"
#include "components/help-marker.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_ps2sdk.h>
#include <imgui/backends/imgui_impl_ps2gskit.h>

extern bool dialog_displaying;

void app_display(ImGuiIO &io, configurator_state *state);
void app_section_information(ImGuiIO &io, configurator_state *state);
void app_section_test(ImGuiIO &io, configurator_state *state);
void app_section_configuration(ImGuiIO &io, configurator_state *state);
void app_section_firmware(ImGuiIO &io, configurator_state *state);
void app_section_about(ImGuiIO &io, configurator_state *state);

void app_no_controller_connected();
void app_begin_dialog();
void app_end_dialog();