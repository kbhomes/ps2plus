#include "irx_imports.h"
#include "ps2plus_updater.h"
#include "rpc.h"

IRX_ID("ps2plupd", 1, 1);
extern struct irx_export_table _exp_ps2plupd;

int _start(int argc, char * argv[]) {
    if (RegisterLibraryEntries(&_exp_ps2plupd) != 0)
        return MODULE_NO_RESIDENT_END;

    static ps2plus_rpc_command_handler handlers[NUM_PS2PLUS_RPC_COMMANDS] = {
        NULL,                       // PS2Plus_Init,
        &command_get_version,       // PS2Plus_GetVersion,
        &command_get_configuration, // PS2Plus_GetConfiguration,
        NULL,                       // PS2Plus_SetConfiguration,
        NULL,                       // PS2Plus_DisableEnableConfiguration,
        NULL,                       // PS2Plus_RestoreConfigurationDefaults,
    };

    ps2plus_rpc_init(handlers);

    return MODULE_RESIDENT_END;
}