#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_restore_configuration_defaults[] = { 0xFF, 0x41, 0x5A };

void command_restore_configuration_defaults(ps2plman_rpc_packet *packet) {
    ps2plman_rpc_command_restore_configuration_defaults *command = &packet->restore_configuration_defaults;

    // This command has no payload
    packet->ok = ps2plman_spi_transmit_mock(0x73, NULL, NULL, 0, rx_mock_restore_configuration_defaults);
}