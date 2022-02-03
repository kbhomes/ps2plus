#include "../irx_imports.h"
#include "../ps2plman.h"
#include "../spi.h"

static uint8_t rx_mock_get_version[] = { 0xFF, 0x41, 0x5A, 0x00, PDT_Array, 0x06, 'H', 'e', 'l', 'l', 'o', '!' };

void command_get_version(ps2plus_rpc_packet *packet) {
    ps2plus_rpc_command_get_version *command = &packet->get_version;

    // The largest possible payload from this command is about 35 bytes, in the case
    // of requesting the microcontroller version string which has a max length of 32.
    static uint8_t tx_[64];
    static uint8_t rx_[64];

    //  | Index | Size | TX         | RX                              |
    //  |-------|------|------------|---------------------------------|
    //  | 0     | 1    | Version ID | --                              |
    //  | 1     | 1    | --         | Primitive type                  |
    //  | 2     | 1    | --         | Array length (if type is array) |
    //  | ...   | ...  | --         | ...                             |

    // First byte of the payload is always the requested version ID
    tx_[0] = command->version_id;

    // First pass to get the size of the version payload
    {
        // The first 3 bytes (and specifically RX indices 1 and 2) are required to 
        // calculate the size of the primitive data that the PS2+ wants to return.
        packet->ok = ps2plus_spi_transmit_mock(0x70, tx_, rx_, 3, rx_mock_get_version);

        // "Pre-load" the primitive type and possible array length into the response buffer
        command->version_response.type = rx_[1];
        command->version_response.array.length = rx_[2];
    }

    // Second pass to get the real payload, based on the data length calculated from the first pass
    if (packet->ok) {
        size_t primitive_size = primitive_data_length(&command->version_response);
        ps2plus_spi_transmit_mock(0x70, tx_, rx_, 1 + primitive_size, rx_mock_get_version);

        // Deserialize the primitive data from the payload response
        primitive_data_deserialize(&command->version_response, rx_ + 1);
        
        // Handle string/array responses by copying the array data into the packet's dedicated buffer area.
        // This is required since the original array pointer wouldn't be properly handled when data is 
        // transfered via RPC back to the EE from the IOP.
        if (command->version_response.type == PDT_Array) {
            // Copy the array into the response buffer
            memcpy(
                command->version_buffer, 
                command->version_response.array.data, 
                MIN(sizeof(command->version_buffer), command->version_response.array.length)
            );

            command->version_response.array.data = NULL;
        }
    }
}