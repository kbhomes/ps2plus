#include <ps2plus_common.h>

#include <loadfile.h>
#include <kernel.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <stdio.h>
#include <string.h>
#include <tamtypes.h>

static SifRpcClientData_t rpc_client_data;
static ps2plus_updater_rpc_packet rpc_packet;

static void loadModules(void) {
    int ret;

    ret = SifLoadModule("rom0:PS2PLUPD", 0, NULL);
    if (ret < 0) {
        printf("SifLoadModule ps2plupd failed: %d\n", ret);
        SleepThread();
    } else {
        printf("Loaded ps2plupd module");
    }
}

int main() {
    int ret;

    loadModules();
    SifInitRpc(0);

    // Attempt to bind to the PS2+ Updater's RPC server
    do {
        if (SifBindRpc(&rpc_client_data, PS2PLUPD_RPC_BIND_ID, 0) < 0) {
            return -1;
        }
        nopdelay();
    } while(!rpc_client_data.server);

    // Make a function call to the RPC server
    rpc_packet.command = PS2PlusCommandInit;
    memcpy(&rpc_packet.data, "Hello", 6);
    ret = SifCallRpc(&rpc_client_data, 1, 0, &rpc_packet, sizeof(ps2plus_updater_rpc_packet), &rpc_packet, sizeof(ps2plus_updater_rpc_packet), NULL, NULL);
    
    if (ret >= 0) {
        printf("Result: %d\n", ret);
        printf("Response: %s\n", rpc_packet.data);
    } else {
        printf("Result: %d\n", ret);
    }

    SleepThread();
    return 0;
}
