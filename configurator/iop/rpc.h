#ifndef IOP_RPC_H
#define IOP_RPC_H

#include "irx_imports.h"

#include <ps2plus_common.h>

typedef void (*ps2plus_rpc_command_handler)(ps2plus_rpc_packet *packet);

void ps2plus_rpc_init(ps2plus_rpc_command_handler *handlers);

#endif