#ifndef PLATFORMS_SPI_PLAYSTATION_H
#define PLATFORMS_SPI_PLAYSTATION_H

#include <stdint.h>
#include <stdbool.h>

void platform_spi_playstation_ack();

bool platform_spi_playstation_selected();

bool platform_spi_playstation_data_available();

uint8_t platform_spi_playstation_read();

void platform_spi_playstation_write(uint8_t);

uint8_t platform_spi_playstation_transmit(uint8_t value);

#endif /* PLATFORMS_SPI_PLAYSTATION_H */