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
    
    printf("Loading padman module (system) - \n");
    ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
    printf("[%d] returned\n", ret);

    printf("Loading padman module (system) - \n");
    ret = SifLoadModule("rom0:PADMAN", 0, NULL);
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

    // Setup the graphics and ImGui systems
    bool hires = false;
    bool use_texture_manager = true;
    GSGLOBAL *global = gfx_init(hires, use_texture_manager);
    gfx_imgui_init(global);
    ImGuiIO &io = ImGui::GetIO();

    // TODO: Actually populate state based on connected controllers
    state.current_controller = &state.controllers[0];
    state.current_controller->connected = true;

    while (1) {
        gfx_render_begin(global, hires, use_texture_manager);
        gfx_render_clear(global, GS_SETREG_RGBA(0x30, 0x30, 0x40, 0x80));

        pad_get_status(&state.pad_status);
        
        app_display(io, &state);
        gfx_render_end(global, hires, use_texture_manager);
    }

    return 0;
}
