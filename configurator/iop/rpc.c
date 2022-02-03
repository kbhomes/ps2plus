#include "rpc.h"

// RPC thread and server
static s32 rpc_thread_id;
static SifRpcDataQueue_t rpc_data_queue;
static SifRpcServerData_t rpc_server_data;
static u32 rpc_server_buffer[32];

ps2plman_rpc_command_handler *command_handlers;

static void *_rpc_server(int fno, void *buffer, int length) {
    ps2plman_rpc_packet *packet = (ps2plman_rpc_packet *)buffer;
    ps2plman_rpc_command command = packet->command;

    if (command_handlers && command_handlers[command]) {
        command_handlers[command](packet);
    } else {
        printf("[ps2plman] Command: Unknown (%d)\n", command);
    }

    return buffer; 
}

static void _rpc_thread(void *arg) {
    if (sceSifCheckInit() == 0) {
        printf("[ps2plman] SIF not initialized\n");
        sceSifInit();
    }

    sceSifInitRpc(0);
    sceSifSetRpcQueue(&rpc_data_queue, GetThreadId());
    sceSifRegisterRpc(&rpc_server_data, PS2PLMAN_RPC_BIND_ID, &_rpc_server, rpc_server_buffer, NULL, NULL, &rpc_data_queue);
    sceSifRpcLoop(&rpc_data_queue);
}

void ps2plman_rpc_init(ps2plman_rpc_command_handler *handlers) {
    command_handlers = handlers;

    // RPC Server
    iop_thread_t rpc_thread;
    rpc_thread.attr = TH_C;
    rpc_thread.thread = &_rpc_thread;
    rpc_thread.stacksize = 0x800;
    rpc_thread.priority = PS2PLMAN_THREAD_PRIO;
    rpc_thread_id = CreateThread(&rpc_thread);
    StartThread(rpc_thread_id, NULL);
}