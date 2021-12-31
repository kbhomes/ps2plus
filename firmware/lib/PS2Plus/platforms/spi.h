#ifndef PLATFORMS_SPI_H
#define PLATFORMS_SPI_H

#include <stdint.h>
#include <stdbool.h>

bool platform_spi_selected();

bool platform_spi_data_available();

uint8_t platform_spi_read();

void platform_spi_write(uint8_t);

#endif /* PLATFORMS_SPI_H */