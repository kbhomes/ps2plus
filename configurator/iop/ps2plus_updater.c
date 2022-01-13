#include "irx_imports.h"
#include "ps2plus_updater.h"

IRX_ID("PS2+ Firmware Updater Support", 1, 1);

extern struct irx_export_table _exp_ps2plupd;

// RPC thread and server
static s32 rpc_thread_id;
static SifRpcDataQueue_t rpc_data_queue;
static SifRpcServerData_t rpc_server_data;
static u32 rpc_server_buffer[32];

static void *rpcServer(int fno, void *buffer, int length) {
    u32 *data = (u32 *)buffer;
    u32 command = data[0];

    switch (command) {
        default:
            printf("Received command: (%03x)\n", (int)data[0]);
            break;
    }

    return buffer;
}

static void rpcThread(void *arg) {
    if (sceSifCheckInit() == 0) {
        printf("SIF not initialized.\n");
        sceSifInit();
    }

    sceSifInitRpc(0);
    sceSifSetRpcQueue(&rpc_data_queue, GetThreadId());
    sceSifRegisterRpc(&rpc_server_data, PS2PLUPD_RPC_BIND_ID, &rpcServer, rpc_server_buffer, NULL, NULL, &rpc_data_queue);
    sceSifRpcLoop(&rpc_data_queue);
}

void ps2PlusUpdaterInit(void) {
    iop_thread_t rpc_thread;

    // RPC Server
    rpc_thread.attr = TH_C;
    rpc_thread.thread = &rpcThread;
    rpc_thread.stacksize = 0x800;
    rpc_thread.priority = PS2PLUPD_THREAD_PRIO;
    rpc_thread_id = CreateThread(&rpc_thread);
    StartThread(rpc_thread_id, NULL);
}

int _start(int argc, char * argv[]) {
    if (RegisterLibraryEntries(&_exp_ps2plupd) != 0)
        return MODULE_NO_RESIDENT_END;

    ps2PlusUpdaterInit();

    return MODULE_RESIDENT_END;
}