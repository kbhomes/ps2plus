#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_reboot_controller[] = { 0xFF, 0x41, 0x5A };

void command_reboot_controller(ps2plman_rpc_packet *packet) {
    ps2plman_rpc_command_reboot_controller *command = &packet->reboot_controller;

    // This command has no payload
    packet->ok = ps2plman_spi_transmit_mock(0x7D, NULL, NULL, 0, rx_mock_reboot_controller);
}