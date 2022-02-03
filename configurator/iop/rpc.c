#include "rpc.h"

// RPC thread and server
static s32 rpc_thread_id;
static SifRpcDataQueue_t rpc_data_queue;
static SifRpcServerData_t rpc_server_data;
static u32 rpc_server_buffer[32];

ps2plus_rpc_command_handler *command_handlers;

static void *_rpc_server(int fno, void *buffer, int length) {
    ps2plus_rpc_packet *packet = (ps2plus_rpc_packet *)buffer;
    ps2plus_rpc_command command = packet->command;

    if (command_handlers && command_handlers[command]) {
        command_handlers[command](packet);
    } else {
        printf("[ps2plman] Command: Unknown (%d)\n", command);
    }

    return buffer; 

    // switch (command) {
    //     case PS2Plus_Init:
    //         printf("[ps2plman] Command: PS2Plus_Init\n");
    //         padConnect();
    //         return buffer;

    //     case PS2Plus_GetVersion:
    //         printf("[ps2plman] Command: PS2Plus_GetVersion\n");
    //         ps2plus_get_version(packet);
    //         return buffer;

    //     case PS2Plus_GetConfiguration:
    //         printf("[ps2plman] Command: PS2Plus_GetConfiguration\n");
    //         return buffer;

    //     case PS2Plus_SetConfiguration:
    //         printf("[ps2plman] Command: PS2Plus_SetConfiguration\n");
    //         return buffer;

    //     case PS2Plus_DisableEnableConfiguration:
    //         printf("[ps2plman] Command: PS2Plus_DisableEnableConfiguration\n");
    //         return buffer;

    //     case PS2Plus_RestoreConfigurationDefaults:
    //         printf("[ps2plman] Command: PS2Plus_RestoreConfigurationDefaults\n");
    //         return buffer;

    //     default:
    //         printf("[ps2plman] Command: Unknown (%d)\n", command);
    //         break;
    // }

    // return buffer;
}

static void _rpc_thread(void *arg) {
    if (sceSifCheckInit() == 0) {
        printf("[ps2plman] SIF not initialized\n");
        sceSifInit();
    }

    sceSifInitRpc(0);
    sceSifSetRpcQueue(&rpc_data_queue, GetThreadId());
    sceSifRegisterRpc(&rpc_server_data, PS2PLUPD_RPC_BIND_ID, &_rpc_server, rpc_server_buffer, NULL, NULL, &rpc_data_queue);
    sceSifRpcLoop(&rpc_data_queue);
}

void ps2plus_rpc_init(ps2plus_rpc_command_handler *handlers) {
    command_handlers = handlers;

    // RPC Server
    iop_thread_t rpc_thread;
    rpc_thread.attr = TH_C;
    rpc_thread.thread = &_rpc_thread;
    rpc_thread.stacksize = 0x800;
    rpc_thread.priority = PS2PLUPD_THREAD_PRIO;
    rpc_thread_id = CreateThread(&rpc_thread);
    StartThread(rpc_thread_id, NULL);
}