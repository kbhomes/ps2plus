#pragma once

#include <state.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_ps2sdk.h>
#include <imgui/backends/imgui_impl_ps2gskit.h>
#include <ui/widgets/widget.h>

void app_display(ImGuiIO &io, configurator_state *state);
void app_section_information(ImGuiIO &io, configurator_state *state);
void app_section_configuration(ImGuiIO &io, configurator_state *state);
void app_section_firmware(ImGuiIO &io, configurator_state *state);
void app_section_about(ImGuiIO &io, configurator_state *state);