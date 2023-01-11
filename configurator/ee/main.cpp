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

#include "app/views/app.h"
#include "app/state.h"

#include "ui/gfx.h"
#include "ui/pad.h"

#define NEWLIB_PORT_AWARE
#include <fileXio_rpc.h>
#include <io_common.h>
#include <time.h>

bool is_on_hw() {
    float num = 2.0f;
    asm __volatile__
    (
        "QMTC2 %1, $vf1\n" // thr:lat 1:1
        "VDIV $Q, $vf1x, $vf1x\n" // Divide 2.0 / 2.0 
        "VWAITQ\n" // Q will be 1 after this
        "VDIV $Q, $vf0x, $vf1x\n" // Divide 0 / 2
        "VMULQ $vf1, $vf1, $Q\n" // Multiply 2 by Q and store in vf1
        "SQC2 $vf1, %0\n"
    :"=m"(num):"r"(num));
    return num == 2.0f;
}

bool should_mock = !is_on_hw();

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

float get_time() {
    return (float)(clock()) / CLOCKS_PER_SEC;
}

// void handle_update() {
//     configurator_ps2plus_controller *controller = state.current_controller;
//     if (!controller || !controller->connected) {
//         return;
//     }

//     /*
//     We want to perform the update in the following way:

//     - Reboot the controller:
//       - Send the reboot PS2+ command to the controller; it will instantly reboot
//       - Query the bootloader update status every Xms until it is `BLStatusPending` to determine when the bootloader is ready
//       - If the bootloader is not ready within 1 second, error
//     - Send update records:
//       - Send the current update record
//       - Query the bootloader update status every Xms until it is `BLStatusOk` to determine when the bootloader is ready for another record
//       - If the bootloader is not ready within 1 second, error
//     - Complete the update:
//       - Prepare for rebooted controller
//     */

//     // We want to fill up as much time as possible in the frame with update commands,
//     // but we don't want to bring the UI down to < 5fps.
//     float max_frame_time = 1.0 / 5;
//     float frame_start = get_time();

//     while (get_time() - frame_start < max_frame_time) {
//         float dt_check = get_time() - controller->update.last_check_time;
//         int ret;
//         ps2plus_bootloader_status bl_status;
//         ps2plus_bootloader_error bl_error;
        
//         if (controller->update.status == StatusPending) {
//             controller->update.status = StatusRebooting;
//             controller->update.last_check_time = get_time();
//             PS2Plus::Gamepad::StopAll();

//             // Begin rebooting the controller
//             printf("[handle_update] ps2plman_reboot_controller() = %d\n", ps2plman_reboot_controller());
//         } else if (controller->update.status == StatusRebooting) {
//             // Check if the controller is in `BLStatusPending` mode, indicating the bootloader can accept firmware updates
//             ret = ps2plman_bootloader_query_firmware_update_status(&bl_status, &bl_error);
//             printf("[handle_update] ps2plman_bootloader_query_firmware_update_status(&bl_status, &bl_error) = %d\n", ret);

//             if (bl_status == BLStatusPending) {
//                 printf("[handle_update] Ready to update!\n");
//                 controller->update.status = StatusUpdating;
//                 controller->update.last_check_time = get_time();
//                 controller->update.last_record_index = 0;
//                 controller->update.total_records = controller->update.firmware->GetRecords().size();
//             } else if (dt_check > 5.0) {
//                 printf("[handle_update] Timed out waiting for bootloader ready\n");
//                 controller->update.status = StatusFailed;
//                 PS2Plus::Gamepad::StartAll();
//             }
//         } else if (controller->update.status == StatusUpdating) {
//             controller->update.status = StatusUpdatePending;
//             controller->update.last_check_time = get_time();

//             // Send the current record
//             ps2plus_bootloader_update_record *record = controller->update.firmware->GetRecords()[controller->update.last_record_index].get();
//             ret = ps2plman_bootloader_update_firmware_data(record);
//             printf("[handle_update] ps2plman_bootloader_update_firmware_data(record) = %d\n", ret);
//         } else if (controller->update.status == StatusUpdatePending) {
//             if (controller->update.last_record_index + 1 == controller->update.total_records) {
//                 // Final record won't have a response
//                 controller->update.status = StatusCompleted;
//                 controller->update.last_check_time = get_time();
//                 PS2Plus::Gamepad::StartAll();
//             } else {
//                 // Check if the controller is in `BLStatusOk` mode, indicating the bootloader can accept the next record
//                 ret = ps2plman_bootloader_query_firmware_update_status(&bl_status, &bl_error);
//                 printf("[handle_update] ps2plman_bootloader_query_firmware_update_status(&bl_status, &bl_error) = %d\n", ret);

//                 if (ret == PS2PLMAN_RET_OK) {
//                     if (bl_status == BLStatusOk) {
//                         controller->update.status = StatusUpdating;
//                         controller->update.last_check_time = get_time();
//                         controller->update.last_record_index++;
//                     } else if (bl_status == BLStatusError) {
//                         printf("[handle_update] Error processing record %d: %d\n", controller->update.last_record_index, bl_error);
//                         controller->update.status = StatusFailed;
//                         PS2Plus::Gamepad::StartAll();
//                     } else if (dt_check > 5.0) {
//                         printf("[handle_update] Timed out waiting for bootloader ready\n");
//                         controller->update.status = StatusFailed;
//                         PS2Plus::Gamepad::StartAll();
//                     }
//                 }
//             }
//         } else if (controller->update.status == StatusCompleted) {
//             // controller->update.status = StatusNone;
//             break;
//         } else if (controller->update.status == StatusFailed) {
//             // controller->update.status = StatusNone;
//             break;
//         } else {
//             break;
//         }
//     }
// }

int main(int argc, char **argv) {
    int ret;
    
    load_modules();
    ps2plman_init();

    PS2Plus::App::State& state = PS2Plus::App::GetState();
    PS2Plus::Gamepad::Initialize();
    PS2Plus::Gamepad::StartAll();
    PS2Plus::Graphics::Initialize(state.video_mode());
    
    for (auto controller : state.controllers()) {
        printf("Connecting to PS2+ on port %d - %s\n", controller->port(), controller->Connect() ? "success" : "failed");
    }
    state.SelectController(0);

    // Mock the controllers
    if (should_mock) {
        state.MockController(0);
    }

    while (1) {
        // handle_update();

        // Update the gamepad
        state.UpdateGamepads();
        
        // Update the graphics state
        PS2Plus::Graphics::UpdateGamepad(state.gamepad_summary());
        PS2Plus::Graphics::BeginFrame();
        PS2Plus::App::Views::MainView();
        PS2Plus::Graphics::EndFrame();
    }

    return 0;
}
