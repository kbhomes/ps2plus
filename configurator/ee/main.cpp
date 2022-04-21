#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <loadfile.h>
#include <sbv_patches.h>
#include <sifrpc.h>
#include <stdio.h>

#include <ps2plus_common.h>
#include <shared/config.h>

#include "irx_builtins.h"
#include "libps2plman.h"
#include "app/app.h"

#include "state.h"
#include "ui/custom_font.h"
#include "ui/drawing/drawing.h"
#include "ui/gfx.h"
#include "ui/pad.h"
#include "ui/widgets/widget.h"

#define NEWLIB_PORT_AWARE
#include <fileXio_rpc.h>
#include <io_common.h>

bool is_reading_ps2_state = false;
bool should_mock = false;

configurator_state state;

static void load_modules(void) {
    int ret;
        
    // Initialize SIF services
    SifInitRpc(0);
    SifLoadFileInit();
    SifInitIopHeap();

    // Patch is required to load modules from memory
    sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();
    
    printf("Loading freesio2 module (builtin) - (%x,%d) ", (unsigned int)freesio2_irx, size_freesio2_irx);
    SifExecModuleBuffer(freesio2_irx, size_freesio2_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);

    printf("Loading freepad module (builtin) - (%x,%d) ", (unsigned int)freepad_irx, size_freepad_irx);
    SifExecModuleBuffer(freepad_irx, size_freepad_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);

    printf("Loading iomanX module (builtin) - (%x,%d) ", (unsigned int)iomanX_irx, size_iomanX_irx);
    SifExecModuleBuffer(iomanX_irx, size_iomanX_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);

    printf("Loading fileXio module (builtin) - (%x,%d) ", (unsigned int)fileXio_irx, size_fileXio_irx);
    SifExecModuleBuffer(fileXio_irx, size_fileXio_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);

    printf("Loading ps2plman module (builtin) - (%x,%d) ", (unsigned int)ps2plman_irx, size_ps2plman_irx);
    SifExecModuleBuffer(ps2plman_irx, size_ps2plman_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);
}

void update_controllers() {
    int ret;
    bool connected = true;

    // Stop the gamepad
    PS2Plus::Gamepad::Stop();

    if ((ret = ps2plman_get_firmware_version(&state.controllers[0].versions.firmware)) != PS2PLMAN_RET_OK) {
        printf("Error retrieving PS2+ firmware version: %d\n", ret);
        connected = false;
    }

    if ((ret = ps2plman_get_microcontroller_version(state.controllers[0].versions.microcontroller, NULL, sizeof(state.controllers[0].versions.microcontroller) - 1)) != PS2PLMAN_RET_OK) {
        printf("Error retrieving PS2+ microcontroller version: %d\n", ret);
        connected = false;
    }

    if ((ret = ps2plman_get_configuration_version(&state.controllers[0].versions.configuration)) != PS2PLMAN_RET_OK) {
        printf("Error retrieving PS2+ configuration version: %d\n", ret);
        connected = false;
    }
    
    state.controllers[0].connected = connected;

    // If the controller successfully connected, retrieve all configurations
    if (connected) {
        ps2plman_get_configuration(CONFIGURATION_ID(enable_button_remapping), &state.controllers[0].configuration.enable_button_remapping);
        for (size_t i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
            ps2plman_get_configuration(CONFIGURATION_ID(button_remapping) + i, &state.controllers[0].configuration.button_remapping[i]);
        }
        ps2plman_get_configuration(CONFIGURATION_ID(enable_joystick_axis_range_remapping), &state.controllers[0].configuration.enable_joystick_axis_range_remapping);
        for (size_t i = 0; i < NUM_JOYSTICK_AXIS_RANGES; i++) {
            ps2plman_get_configuration(CONFIGURATION_ID(joystick_axis_range_remapping) + i, &state.controllers[0].configuration.joystick_axis_range_remapping[i]);
        }
        ps2plman_get_configuration(CONFIGURATION_ID(joystick_deadzone_left), &state.controllers[0].configuration.joystick_deadzone_left);
        ps2plman_get_configuration(CONFIGURATION_ID(joystick_deadzone_right), &state.controllers[0].configuration.joystick_deadzone_right);
        ps2plman_get_configuration(CONFIGURATION_ID(joystick_digital_enable_left), &state.controllers[0].configuration.joystick_digital_enable_left);
        ps2plman_get_configuration(CONFIGURATION_ID(joystick_digital_enable_right), &state.controllers[0].configuration.joystick_digital_enable_right);
        ps2plman_get_configuration(CONFIGURATION_ID(joystick_digital_threshold_left), &state.controllers[0].configuration.joystick_digital_threshold_left);
        ps2plman_get_configuration(CONFIGURATION_ID(joystick_digital_threshold_right), &state.controllers[0].configuration.joystick_digital_threshold_right);

        // Dump the configuration
        printf("[config] enable_button_remapping = %d\n", state.controllers[0].configuration.enable_button_remapping.boolean);
        for (uint8_t b = 0; b < NUM_DIGITAL_BUTTONS; b++) {
            printf("[config] button_remapping[%s] = %s\n", 
                ps2plus_controller_digital_button_name((ps2plus_controller_digital_button)b), 
                ps2plus_controller_digital_button_name((ps2plus_controller_digital_button)state.controllers[0].configuration.button_remapping[b].uint8));
        }
        printf("[config] enable_joystick_axis_range_remapping = %d\n", state.controllers[0].configuration.enable_joystick_axis_range_remapping.boolean);
        for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
            printf("[config] joystick_axis_range_remapping[%s] = %u\n", 
                ps2plus_controller_joystick_axis_range_name((ps2plus_controller_joystick_axis_range)a), 
                state.controllers[0].configuration.joystick_axis_range_remapping[a].uint8);
        }
        printf("[config] joystick_deadzone_left = %u\n", state.controllers[0].configuration.joystick_deadzone_left.uint8);
        printf("[config] joystick_deadzone_right = %u\n", state.controllers[0].configuration.joystick_deadzone_right.uint8);
        printf("[config] joystick_digital_enable_left = %d\n", state.controllers[0].configuration.joystick_digital_enable_left.boolean);
        printf("[config] joystick_digital_enable_right = %d\n", state.controllers[0].configuration.joystick_digital_enable_right.boolean);
        printf("[config] joystick_digital_threshold_left = %u\n", state.controllers[0].configuration.joystick_digital_threshold_left.uint8);
        printf("[config] joystick_digital_threshold_right = %u\n", state.controllers[0].configuration.joystick_digital_threshold_right.uint8);
    }

    // Start reading controller data again
    PS2Plus::Gamepad::Start();
}

void handle_update() {
    configurator_ps2plus_controller *controller = state.current_controller;
    if (!controller || !controller->connected) {
        return;
    }

    if (controller->update.status == StatusPending) {
        controller->update.status = StatusRebooting;
        controller->update.last_check_time = ImGui::GetTime();
        // TODO: Do the reboot
    } else if (controller->update.status == StatusRebooting) {
        if (ImGui::GetTime() - controller->update.last_check_time > 5.f) {
            controller->update.status = StatusUpdating;
            controller->update.last_check_time = ImGui::GetTime();
            controller->update.last_record_index = 0;
            controller->update.total_records = controller->update.firmware->GetRecords().size();
            // TODO: Start the updating
        }
    } else if (controller->update.status == StatusUpdating) {
        // DEV: Advance progress every 0.1s
        float __delta = ImGui::GetTime() - controller->update.last_check_time;
        
        if (__delta > 0.01f) {
            controller->update.last_check_time = ImGui::GetTime();
            controller->update.last_record_index++;
            // TODO: Actually send the update record
        }

        if (controller->update.last_record_index == controller->update.total_records) {
            controller->update.status = StatusCompleted;
            controller->update.last_check_time = ImGui::GetTime();
        }
    } else if (controller->update.status == StatusCompleted) {

    } else if (controller->update.status == StatusFailed) {

    }
}

int main(int argc, char **argv) {
    int ret;
    
    load_modules();
    ps2plman_init();

    PS2Plus::Gamepad::Initialize();
    PS2Plus::Gamepad::Start();
    PS2Plus::Graphics::Initialize();
    
    update_controllers();
    state.current_controller = &state.controllers[0];

    // Mock the controllers
    if (should_mock) {
        state.current_controller->connected = true;
        state.current_controller->versions.firmware = 30;
        state.current_controller->versions.configuration = 1;
        strncpy(state.current_controller->versions.microcontroller, "PIC18F46K42", 12);
        
        primitive_data_initialize_boolean(&state.current_controller->configuration.enable_button_remapping, false);
        for (size_t i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
            primitive_data_initialize_uint8(&state.current_controller->configuration.button_remapping[i], i);
        }
        primitive_data_initialize_boolean(&state.current_controller->configuration.enable_joystick_axis_range_remapping, false);
        for (size_t i = 0; i < NUM_JOYSTICK_AXIS_RANGES; i++) {
            primitive_data_initialize_uint8(&state.current_controller->configuration.joystick_axis_range_remapping[i], (i % 3 == 0) ? 0 : (i % 3 == 1) ? 127 : 255);
        }
        primitive_data_initialize_uint8(&state.current_controller->configuration.joystick_deadzone_left, 0);
        primitive_data_initialize_uint8(&state.current_controller->configuration.joystick_deadzone_right, 0);
        primitive_data_initialize_boolean(&state.current_controller->configuration.joystick_digital_enable_left, false);
        primitive_data_initialize_boolean(&state.current_controller->configuration.joystick_digital_enable_right, false);
        primitive_data_initialize_uint8(&state.current_controller->configuration.joystick_digital_threshold_left, 0x40);
        primitive_data_initialize_uint8(&state.current_controller->configuration.joystick_digital_threshold_right, 0x40);
    }

    while (1) {
        handle_update();

        // Update the gamepad
        state.pad_status = PS2Plus::Gamepad::Read();
        
        // Update the graphics state
        PS2Plus::Graphics::UpdateGamepad(state.pad_status);
        PS2Plus::Graphics::BeginFrame(GS_SETREG_RGBA(0x30, 0x30, 0x40, 0x80));
        app_display(ImGui::GetIO(), &state);
        PS2Plus::Graphics::EndFrame();
    }

    return 0;
}
