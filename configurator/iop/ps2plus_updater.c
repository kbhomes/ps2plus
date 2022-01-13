#include "irx_imports.h"
#include "ps2plus_updater.h"

IRX_ID("ps2plupd", 1, 1);

extern struct irx_export_table _exp_ps2plupd;

// RPC thread and server
static s32 rpc_thread_id;
static SifRpcDataQueue_t rpc_data_queue;
static SifRpcServerData_t rpc_server_data;
static u32 rpc_server_buffer[32];

static void padConnect() {
    // sio2_pad_transfer_init();
}

static void *rpcServer(int fno, void *buffer, int length) {
    ps2plus_updater_rpc_packet *packet = (ps2plus_updater_rpc_packet *)buffer;
    ps2plus_updater_rpc_command command = packet->command;

    switch (command) {
        case PS2PlusCommandInit:
            printf("[ps2plupd] Command: PS2PlusCommandInit\n");
            padConnect();
            memcpy(packet->data, "World!", 7);
            return buffer;

        default:
            printf("[ps2plupd] Command: Unknown (%d)\n", command);
            break;
    }

    return buffer;
}

static void rpcThread(void *arg) {
    if (sceSifCheckInit() == 0) {
        printf("[ps2plupd] SIF not initialized\n");
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