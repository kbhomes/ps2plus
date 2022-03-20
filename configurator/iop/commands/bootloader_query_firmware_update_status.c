#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_bootloader_query_firmware_update_status[23] = { 0xFF, 0x41, 0x5A, BLStatusOk, 0, 0 };

void command_bootloader_query_firmware_update_status(ps2plman_rpc_packet *packet) {
    ps2plman_rpc_command_bootloader_query_firmware_update_status *command = &packet->bootloader_query_firmware_update_status;

    // The largest possible payload from this command is 3 bytes
    static uint8_t tx_[3];
    static uint8_t rx_[3];

    //  | Index | Size | TX | RX                       |
    //  |-------|------|----|--------------------------|
    //  | 0     | 1    | -- | Bootloader update status |
    //  | 1     | 2    | -- | Bootloader error code    |

    // Transmit the payload
    packet->ok = ps2plman_spi_transmit_mock(0x7F, tx_, rx_, 3, rx_mock_bootloader_query_firmware_update_status);
    command->status = rx_[0];
    command->error = rx_[1] | (rx_[2] << 8);
}