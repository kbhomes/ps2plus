#include "irx_builtins.h"

#include <ps2plus_common.h>

#include <debug.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <loadfile.h>
#include <sbv_patches.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <stdio.h>
#include <string.h>
#include <tamtypes.h>
#include <unistd.h>

static SifRpcClientData_t rpc_client_data;
static ps2plus_rpc_packet rpc_packet;

static void loadModules(void) {
    int ret;

    printf("Loading sio2man module (builtin) - (%x,%d) ", (unsigned int)sio2man_irx, size_sio2man_irx);
    SifExecModuleBuffer(sio2man_irx, size_sio2man_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);

    printf("Loading ps2plman module (builtin) - (%x,%d) ", (unsigned int)ps2plman_irx, size_ps2plman_irx);
    SifExecModuleBuffer(ps2plman_irx, size_ps2plman_irx, 0, NULL, &ret);
    printf("[%d] returned\n", ret);
}

int main(int argc, char *argv[]) {
    int ret;

	//Reboot IOP
	SifInitRpc(0);
	while(!SifIopReset("", 0)){};
	while(!SifIopSync()){};
    
	//Initialize SIF services
	SifInitRpc(0);
	SifLoadFileInit();
	SifInitIopHeap();
	sbv_patch_enable_lmb();

    // Load modules for the program
    loadModules();

    // Attempt to bind to the PS2+ Updater's RPC server
    printf("Attempting to bind to ps2plman RPC server\n");
    do {
        if (SifBindRpc(&rpc_client_data, PS2PLUPD_RPC_BIND_ID, 0) < 0) {
            return -1;
        }
        nopdelay();
    } while(!rpc_client_data.server);

    // Make a function call to the RPC server
    printf("Making RPC call to ps2plman\n");

    rpc_packet.command = PS2Plus_Init;
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    printf("Result: %d\n", ret);
    
    rpc_packet.command = PS2Plus_GetVersion;
    rpc_packet.get_version.version_id = 0x02;
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    if (ret >= 0) {
        printf("Result: %d\n", ret);
        if (rpc_packet.get_version.version_response.type == PDT_Array) {
            printf("Response: %s\n", rpc_packet.get_version.version_buffer);
        } else {
            printf("Response: %08lx\n", rpc_packet.get_version.version_response.uint32);
        }
    } else {
        printf("Result: %d\n", ret);
    }
    
    rpc_packet.command = PS2Plus_GetConfiguration;
    rpc_packet.get_configuration.configuration_id = 0x000C;
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    if (ret >= 0) {
        printf("Result: %d\n", ret);
        if (rpc_packet.get_version.version_response.type == PDT_Array) {
            printf("Response: %s\n", rpc_packet.get_configuration.configuration_buffer);
        } else {
            printf("Response: %08lx\n", rpc_packet.get_configuration.configuration_response.uint32);
        }
    } else {
        printf("Result: %d\n", ret);
    }

    rpc_packet.command = PS2Plus_SetConfiguration;
    rpc_packet.set_configuration.configuration_id = 0x0C;
    primitive_data_initialize_boolean(&rpc_packet.set_configuration.configuration_value, false);
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    printf("Result: %d\n", ret);

    rpc_packet.command = PS2Plus_DisableEnableConfiguration;
    rpc_packet.disable_enable_configuration.configuration_enable = false;
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    printf("Result: %d\n", ret);

    rpc_packet.command = PS2Plus_RestoreConfigurationDefaults;
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    printf("Result: %d\n", ret);

    rpc_packet.command = PS2Plus_RebootController;
    ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plus_rpc_packet), &rpc_packet, sizeof(ps2plus_rpc_packet), NULL, NULL);
    printf("Result: %d\n", ret);

    SleepThread();
    return 0;
}
