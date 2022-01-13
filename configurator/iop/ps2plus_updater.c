#include "irx_imports.h"
#include "ps2plus_updater.h"

IRX_ID("PS2+ Firmware Updater Support", 1, 1);

extern struct irx_export_table _exp_ps2plupd;

void ps2PlusUpdaterInit(void) {
    printf("PS2+ Firmware Updater IRX\n");
}

int _start(int argc, char * argv[]) {
    if (RegisterLibraryEntries(&_exp_ps2plupd) != 0)
        return 1;

    ps2PlusUpdaterInit();

    return 0;
}