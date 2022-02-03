#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_get_configuration[] = { 0xFF, 0x41, 0x5A, 0x00, 0x00, PDT_Boolean, 0xFF };

void command_get_configuration(ps2plus_rpc_packet *packet) {
    ps2plus_rpc_command_get_configuration *command = &packet->get_configuration;

    // The largest possible payload from this command is about 36 bytes, in the case
    // of requesting any string configuration which has a max length of 32.
    static uint8_t tx_[64];
    static uint8_t rx_[64];

    //  | Index | Size | TX               | RX                              |
    //  |-------|------|------------------|---------------------------------|
    //  | 0     | 2    | Configuration ID | --                              |
    //  | 2     | 1    | --               | Primitive type                  |
    //  | 3     | 1    | --               | Array length (if type is array) |
    //  | ...   | ...  | --               | ...                             |

    // First two bytes of the payload is always the requested configuration ID
    tx_[0] = command->configuration_id & 0xFF;
    tx_[1] = (command->configuration_id >> 8) & 0xFF;

    // First pass to get the size of the configuration payload
    {
        // The first 4 bytes (and specifically RX indices 2 and 3) are required to 
        // calculate the size of the primitive data that the PS2+ wants to return.
        packet->ok = ps2plus_spi_transmit_mock(0x71, tx_, rx_, 4, rx_mock_get_configuration);

        // "Pre-load" the primitive type and possible array length into the response buffer
        command->configuration_response.type = rx_[2];
        command->configuration_response.array.length = rx_[3];
    }

    // Second pass to get the real payload, based on the data length calculated from the first pass
    if (packet->ok) {
        size_t primitive_size = primitive_data_length(&command->configuration_response);
        ps2plus_spi_transmit_mock(0x71, tx_, rx_, 2 + primitive_size, rx_mock_get_configuration);

        // Deserialize the primitive data from the payload response
        primitive_data_deserialize(&command->configuration_response, rx_ + 2);
        
        // Handle string/array responses by copying the array data into the packet's dedicated buffer area.
        // This is required since the original array pointer wouldn't be properly handled when data is 
        // transfered via RPC back to the EE from the IOP.
        if (command->configuration_response.type == PDT_Array) {
            // Copy the array into the response buffer
            memcpy(
                command->configuration_buffer, 
                command->configuration_response.array.data, 
                MIN(sizeof(command->configuration_buffer), command->configuration_response.array.length)
            );

            command->configuration_response.array.data = NULL;
        }
    }
}