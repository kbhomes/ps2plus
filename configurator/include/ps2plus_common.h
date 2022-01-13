#ifndef PS2PLUS_COMMON_H
#define PS2PLUS_COMMON_H

#define PS2PLUPD_RPC_BIND_ID 0x7FFFA100
#define PS2PLUPD_THREAD_PRIO 46

typedef enum {
    PS2PlusCommandInit,
    PS2PlusCommandTransmit,
} ps2plus_updater_rpc_command;

typedef struct {
    ps2plus_updater_rpc_command command;
    char data[8];
} ps2plus_updater_rpc_packet __attribute__((aligned(16)));

#endif