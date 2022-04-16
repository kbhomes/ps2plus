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

    printf("[update_controllers] Updating controllers at %f\n", ImGui::GetTime());

    // Stop the gamepad
    printf("[update_controllers] Stopping controllers\n");
    PS2Plus::Gamepad::Stop();
    printf("[update_controllers] Stopped controllers\n");

    printf("[update_controllers] Retrieving PS2+ firmware version\n");
    if ((ret = ps2plman_get_firmware_version(&state.controllers[0].versions.firmware)) != PS2PLMAN_RET_OK) {
        printf("Error retrieving PS2+ firmware version: %d\n", ret);
        connected = false;
    } else {
        printf("[update_controllers] Retrieved PS2+ firmware version: %d\n", state.controllers[0].versions.firmware);
    }

    printf("[update_controllers] Retrieving PS2+ microcontroller version\n");
    if ((ret = ps2plman_get_microcontroller_version(state.controllers[0].versions.microcontroller, NULL, sizeof(state.controllers[0].versions.microcontroller) - 1)) != PS2PLMAN_RET_OK) {
        printf("Error retrieving PS2+ microcontroller version: %d\n", ret);
        connected = false;
    } else {
        printf("[update_controllers] Retrieved PS2+ microcontroller version: %s\n", state.controllers[0].versions.microcontroller);
    }

    printf("[update_controllers] Retrieving PS2+ configuration version\n");
    if ((ret = ps2plman_get_configuration_version(&state.controllers[0].versions.configuration)) != PS2PLMAN_RET_OK) {
        printf("Error retrieving PS2+ configuration version: %d\n", ret);
        connected = false;
    } else {
        printf("[update_controllers] Retrieved PS2+ configuration version: %d\n", state.controllers[0].versions.configuration);
    }
    
    state.controllers[0].connected = connected;

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
    // update_controllers();

    PS2Plus::Gamepad::Initialize();
    PS2Plus::Gamepad::Start();
    PS2Plus::Graphics::Initialize();

    // struct fileXioDevice deviceEntries[32];
    // int deviceEntriesCount;
    // printf("Initializing fileXio\n");
    // printf("- Returned: %d\n", fileXioInit());
    // printf("Getting device list\n");
    // deviceEntriesCount = fileXioGetDeviceList(deviceEntries, 32);
    // printf("- Returned: %d\n", deviceEntriesCount);
    // for (int i = 0; i < deviceEntriesCount; i++) {
    //     printf("deviceEntries[%d].name = %s\n", i, deviceEntries[i].name);
    // }

    // // TODO: Actually populate state based on connected controllers
    state.current_controller = &state.controllers[0];
    // state.current_controller->connected = true;
    // state.current_controller->versions.firmware = 30;
    // state.current_controller->versions.configuration = 1;
    // strncpy(state.current_controller->versions.microcontroller, "PIC18F46K42", 12);

    float last_check_time = ImGui::GetTime();
    float time_delta = 0;

    while (1) {
        // DEV: Code to test PS2+ custom commands that interrupt the main controller thread
        time_delta = ImGui::GetTime() - last_check_time;
        if (time_delta > 5.f) {
            update_controllers();
            last_check_time = ImGui::GetTime();
        }

        handle_update();

        state.pad_status = PS2Plus::Gamepad::Read();
        
        // Update the graphics state
        PS2Plus::Graphics::UpdateGamepad(state.pad_status);
        PS2Plus::Graphics::BeginFrame(GS_SETREG_RGBA(0x30, 0x30, 0x40, 0x80));
        app_display(ImGui::GetIO(), &state);
        PS2Plus::Graphics::EndFrame();
    }

    return 0;
}
