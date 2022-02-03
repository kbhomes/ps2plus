#include "irx_imports.h"
#include "ps2plman.h"
#include "rpc.h"

IRX_ID("ps2plman", 1, 1);
extern struct irx_export_table _exp_ps2plman;

int _start(int argc, char * argv[]) {
    if (RegisterLibraryEntries(&_exp_ps2plman) != 0)
        return MODULE_NO_RESIDENT_END;

    static ps2plman_rpc_command_handler handlers[NUM_PS2PLUS_RPC_COMMANDS] = {
        NULL,                                    // PS2Plus_Init,
        &command_get_version,                    // PS2Plus_GetVersion,
        &command_get_configuration,              // PS2Plus_GetConfiguration,
        &command_set_configuration,              // PS2Plus_SetConfiguration,
        &command_disable_enable_configuration,   // PS2Plus_DisableEnableConfiguration,
        &command_restore_configuration_defaults, // PS2Plus_RestoreConfigurationDefaults,
        &command_reboot_controller,              // PS2Plus_RebootController,
    };

    ps2plman_rpc_init(handlers);

    return MODULE_RESIDENT_END;
}