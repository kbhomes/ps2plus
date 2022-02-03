#ifndef IOP_SPI_H
#define IOP_SPI_H

#include "sio2regs.h"

#include <stdbool.h>

bool ps2plman_spi_transmit(u8 command, u8 *tx_in, u8 *rx_out, size_t payload_size);
bool ps2plman_spi_transmit_mock(u8 command, u8 *tx_in, u8 *rx_out, size_t payload_size, u8 *rx_mock_full);

#endif /* IOP_SPI_H */