#ifndef IOP_PS2PLUS_UPDATER_H
#define IOP_PS2PLUS_UPDATER_H

#include <ps2plus_common.h>
#include <types.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MODNAME "ps2plman"

void command_get_version(ps2plman_rpc_packet *packet);
void command_get_configuration(ps2plman_rpc_packet *packet);
void command_set_configuration(ps2plman_rpc_packet *packet);
void command_disable_enable_configuration(ps2plman_rpc_packet *packet);
void command_restore_configuration_defaults(ps2plman_rpc_packet *packet);
void command_reboot_controller(ps2plman_rpc_packet *packet);
void command_bootloader_update_firmware_data(ps2plman_rpc_packet *packet);
void command_bootloader_query_firmware_update_status(ps2plman_rpc_packet *packet);

#endif /* IOP_PS2PLUS_UPDATER_H */