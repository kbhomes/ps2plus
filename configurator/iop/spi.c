#include "spi.h"

#include "irx_imports.h"
#include "utils.h"

static sio2_transfer_data_t sio_transfer_data;
static u8 tx_buffer[256];
static u8 rx_buffer[256];

// 48 MHz clock divided by 192 (C0h) results in a 250 KHz clock signal
static const int clock_divider = 0xC0;

static void print_hex_array(const u8 *array, size_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%02X ", array[i]);
  }
}

static void transfer_tx_rx(u8 *tx, size_t tx_length, u8 *rx, size_t rx_length) {

  // Initialize SIO2MAN's pad command transfer
  sio2_pad_transfer_init();

  // Setup transfer buffers (confusing: in/out = tx/rx = to/from controller)
  sio_transfer_data.in = tx;
  sio_transfer_data.in_size = tx_length;
  sio_transfer_data.out = rx;
  sio_transfer_data.out_size = rx_length;

  // Setup port and transfer registers
  sio_transfer_data.port_ctrl1[0] = 
    PCTRL0_ATT_LOW_PER(0x05) |
    PCTRL0_ATT_MIN_HIGH_PER(0x05) |
    PCTRL0_BAUD0_DIV(clock_divider) |
    PCTRL0_BAUD1_DIV(clock_divider);
  sio_transfer_data.port_ctrl2[0] = 
    PCTRL1_ACK_TIMEOUT_PER(0x12C) |
    PCTRL1_INTER_BYTE_PER(0) |
    PCTRL1_UNK24(0) |
    PCTRL1_IF_MODE_SPI_DIFF(0);
  sio_transfer_data.regdata[0] = 
    TR_CTRL_PORT_NR(0) |
    TR_CTRL_PAUSE(0) |
    TR_CTRL_TX_MODE_PIO_DMA(0) |
    TR_CTRL_RX_MODE_PIO_DMA(0) |
    TR_CTRL_NORMAL_TR(1) |
    TR_CTRL_SPECIAL_TR(0) |
    TR_CTRL_BAUD_DIV(clock_divider) |
    TR_CTRL_WAIT_ACK_FOREVER(0) |
    TR_CTRL_TX_DATA_SZ(tx_length) |
    TR_CTRL_RX_DATA_SZ(rx_length);

  // Perform the SIO2MAN transfer
  sio2_transfer(&sio_transfer_data);

  // Reset the SIO2MAN transfer to allow another transfer to go through
  sio2_transfer_reset();
}

bool ps2plman_spi_transmit(u8 command, u8 *tx_in, u8 *rx_out, size_t payload_size) {
  // Generate the TX buffer, including the SIO2 header
  tx_buffer[0] = 0x01;
  tx_buffer[1] = command;
  tx_buffer[2] = 0x00;
  if (payload_size) {
    memcpy(tx_buffer + 3, tx_in, payload_size);
  }

  // Clear the RX buffer
  for (size_t i = 0; i < sizeof(rx_buffer); i++) {
    rx_buffer[i] = 0;
  }

  // Complete the transmission
  transfer_tx_rx(tx_buffer, payload_size + 3, rx_buffer, payload_size + 3);

  // Copy the RX payload from the response buffer
  if (payload_size) {
      memcpy(rx_out, rx_buffer + 3, payload_size);
  }

  // Automatic delay after a transmission to allow the controller to register that the packet has completed
  // and prevent the controller or the IOP from being overloaded. This corresponds to about ~120 FPS.
  util_delay(8);

  // Minimal check of the RX validity based on its header
  return rx_buffer[0] == 0xFF && rx_buffer[2] == 0x5A;
}

bool ps2plman_spi_transmit_mock(u8 command, u8 *tx_in, u8 *rx_out, size_t payload_size, u8 *rx_mock_full) {
  // if (rx_mock_full) {
  //   // Generate the TX buffer, including the SIO2 header
  //   tx_buffer[0] = 0x01;
  //   tx_buffer[1] = command;
  //   tx_buffer[2] = 0x00;
  //   if (payload_size) {
  //     memcpy(tx_buffer + 3, tx_in, payload_size);
  //   }

  //   // Copy the mock data into the response payload
  //   if (payload_size) {
  //     memcpy(rx_out, rx_mock_full + 3, payload_size);
  //   }

  //   // printf("[ps2plman] TX: ");
  //   // print_hex_array(tx_buffer, payload_size + 3);
  //   // printf("\n");

  //   // printf("[ps2plman] RX: ");
  //   // print_hex_array(rx_mock_full, payload_size + 3);
  //   // printf("\n");

  //   return rx_mock_full[0] == 0xFF & rx_mock_full[2] == 0x5A;
  // } else {
    return ps2plman_spi_transmit(command, tx_in, rx_out, payload_size);
  // }
}