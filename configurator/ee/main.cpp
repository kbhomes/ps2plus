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

    //Reboot IOP
    SifInitRpc(0);
    while(!SifIopReset("", 0)){};
    while(!SifIopSync()){};
        
    //Initialize SIF services
    SifInitRpc(0);
    SifLoadFileInit();
    SifInitIopHeap();

    // Patch is required to load modules from memory
    sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();
    
    printf("Loading sio2man module (system) - \n");
    ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
    printf("[%d] returned\n", ret);

    printf("Loading padman module (system) - \n");
    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
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
}

int main(int argc, char **argv) {
    int ret;
    
    load_modules();
    pad_init();
    ps2plman_init();
    // update_controllers();
    state.controllers[0].versions.firmware = 30;
    state.controllers[0].versions.configuration = 1;
    strncpy(state.controllers[0].versions.microcontroller, "PIC18F46K42", 12);

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


    // Setup the graphics and ImGui systems
    bool hires = false;
    GSGLOBAL *global = gfx_init(hires);
    gfx_imgui_init(global);
    ImGuiIO &io = ImGui::GetIO();

    // TODO: Actually populate state based on connected controllers
    state.current_controller = &state.controllers[0];
    state.current_controller->connected = true;

    while (1) {
        pad_get_status(&state.pad_status);
        gfx_update_pad(global, &state.pad_status);
        gfx_render_begin(global, hires);
        gfx_render_clear(global, GS_SETREG_RGBA(0x30, 0x30, 0x40, 0x80));
        app_display(io, &state);
        gfx_render_end(global, hires);
    }

    return 0;
}
