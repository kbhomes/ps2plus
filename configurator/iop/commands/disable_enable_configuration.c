#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_disable_enable_configuration[] = { 0xFF, 0x41, 0x5A, 0xFF };

void command_disable_enable_configuration(ps2plman_rpc_packet *packet) {
    ps2plman_rpc_command_disable_enable_configuration *command = &packet->disable_enable_configuration;

    // The only payload size from this command is 1 byte
    static uint8_t tx_[1];
    static uint8_t rx_[1];

    //  | Index | Size | TX                                               | RX              |
    //  |-------|------|--------------------------------------------------|-----------------|
    //  | 0     | 1    | New status (non-zero = enabled, zero = disabled) | Current status  |

    // First and only byte is the new status
    tx_[0] = command->configuration_enable ? 0xFF : 0x00;

    // Transmit the value
    packet->ok = ps2plman_spi_transmit_mock(0x73, tx_, rx_, 1, rx_mock_disable_enable_configuration);
    command->configuration_previous = rx_[0] != 0x00;
}