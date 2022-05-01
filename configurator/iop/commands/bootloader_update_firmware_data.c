#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_bootloader_update_firmware_data[23] = { 0xFF, 0x41, 0x5A, 0x00 };

void command_bootloader_update_firmware_data(ps2plman_rpc_packet *packet) {
    ps2plman_rpc_command_bootloader_update_firmware_data *command = &packet->bootloader_update_firmware_data;

    // The largest possible payload for this command is 23
    static uint8_t tx_[23];
    static uint8_t rx_[23];

    //  | Index | Size | TX                              | RX  |
    //  |-------|------|---------------------------------|-----|
    //  | 0     | 1    | Record type (start/data/end)    | --  |
    //  | 1     | 4    | Target address                  | --  |
    //  | 5     | 1    | Data length (L <= 16)           | --  |
    //  | 6     | L    | Data                            | --  |
    //  | 6 + L | 1    | Checksum                        | --  |

    memset(tx_, 0, sizeof(tx_));
    tx_[0] = command->record.type;
    tx_[1] = (command->record.target_address) & 0xFF;
    tx_[2] = (command->record.target_address >> 8) & 0xFF;
    tx_[3] = (command->record.target_address >> 16) & 0xFF;
    tx_[4] = (command->record.target_address >> 24) & 0xFF;
    tx_[5] = command->record.data_length;
    tx_[6 + command->record.data_length] = command->record.data_checksum;
    for (size_t i = 0; i < command->record.data_length; i++) {
        tx_[6 + i] = command->record.data[i];
    }

    // Transmit the value
    packet->ok = ps2plman_spi_transmit_mock(0x7E, tx_, rx_, 6 + command->record.data_length + 1, rx_mock_bootloader_update_firmware_data);
}