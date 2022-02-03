#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_set_configuration[] = { 0xFF, 0x41, 0x5A, 0x00, 0x00, PDT_Boolean, 0xFF };

void command_set_configuration(ps2plus_rpc_packet *packet) {
    ps2plus_rpc_command_set_configuration *command = &packet->set_configuration;

    // The largest possible payload from this command is about 36 bytes, in the case
    // of setting any string configuration which has a max length of 32.
    static uint8_t tx_[64];
    static uint8_t rx_[64];

    //  | Index | Size | TX                              | RX  |
    //  |-------|------|---------------------------------|-----|
    //  | 0     | 2    | Configuration ID                | --  |
    //  | 2     | 1    | Primitive type                  | --  |
    //  | 3     | 1    | Array length (if type is array) | --  |
    //  | ...   | ...  | --                              | ... |

    // First two bytes of the payload is always the requested configuration ID
    memset(tx_, 0, sizeof(tx_));
    tx_[0] = command->configuration_id & 0xFF;
    tx_[1] = (command->configuration_id >> 8) & 0xFF;

    // Handle array/string values
    if (command->configuration_value.type == PDT_Array) {
        command->configuration_value.array.data = &command->configuration_buffer;
    }

    // Serialize the value directly into the payload
    size_t primitive_size = primitive_data_length(&command->configuration_value);
    primitive_data_serialize(&command->configuration_value, tx_ + 2);

    // Transmit the value
    packet->ok = ps2plus_spi_transmit_mock(0x72, tx_, rx_, 2 + primitive_size, rx_mock_set_configuration);
}