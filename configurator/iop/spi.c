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

static void _transfer_init() {
  // Initialize all control registers
  for (int i = 0; i < 4; i++) {
    sio_transfer_data.port_ctrl1[i] = 0;
    sio_transfer_data.port_ctrl2[i] = 0;
  }

  // TODO: Assuming port 0 for now 
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

  sio_transfer_data.in = NULL;
  sio_transfer_data.in_size = 0;
  sio_transfer_data.in_dma.addr = NULL;
  sio_transfer_data.in_dma.size = 0;
  sio_transfer_data.in_dma.count = 0;

  sio_transfer_data.out = NULL;
  sio_transfer_data.out_size = 0;
  sio_transfer_data.out_dma.addr = NULL;
  sio_transfer_data.out_dma.size = 0;
  sio_transfer_data.out_dma.count = 0;

  // Set all port control bits
  for (int i = 0; i < 4; i++) {
    inl_sio2_portN_ctrl1_set(i, sio_transfer_data.port_ctrl1[i]);
    inl_sio2_portN_ctrl2_set(i, sio_transfer_data.port_ctrl2[i]);
  }
}

static void _transfer_tx_rx(u8 *tx, size_t tx_length, u8 *rx, size_t rx_length) {
  // Disable SIO2 interrupts
  inl_sio2_ctrl_set(0xBC);

  // Setup the SIO2 registers to prepare for a transfer
  inl_sio2_regN_set(0,
    TR_CTRL_PORT_NR(0) |
    TR_CTRL_PAUSE(0) |
    TR_CTRL_TX_MODE_PIO_DMA(0) |
    TR_CTRL_RX_MODE_PIO_DMA(0) |
    TR_CTRL_NORMAL_TR(1) |
    TR_CTRL_SPECIAL_TR(0) |
    TR_CTRL_BAUD_DIV(clock_divider) |
    TR_CTRL_WAIT_ACK_FOREVER(0) |
    TR_CTRL_TX_DATA_SZ(tx_length) |
    TR_CTRL_RX_DATA_SZ(rx_length));
  inl_sio2_regN_set(1, 0);

  // Write the send buffer into the SIO2 TX shift register
  while (tx_length--) {
    inl_sio2_data_out(*tx++);
  }

  // Start the SIO2 transfer
  inl_sio2_ctrl_set(inl_sio2_ctrl_get() | 1);

  // Wait for completion
  while ((inl_sio2_stat6c_get() & (1 << 12)) == 0);

  // Read from the SIO2 RX shift register into the receive buffer
  while (rx_length--) {
    *rx++ = inl_sio2_data_in();
  }
}

bool ps2plman_spi_transmit(u8 command, u8 *tx_in, u8 *rx_out, size_t payload_size) {
  // Generate the TX buffer, including the SIO2 header
  tx_buffer[0] = 0x01;
  tx_buffer[1] = command;
  tx_buffer[2] = 0x00;
    if (payload_size) {
      memcpy(tx_buffer + 3, tx_in, payload_size);
    }

  // Complete the transmission
  _transfer_init();
  _transfer_tx_rx(tx_buffer, payload_size + 3, rx_buffer, payload_size + 3);

  // Copy the RX payload from the response buffer
  if (payload_size) {
      memcpy(rx_out, rx_buffer + 3, payload_size);
  }

  // printf("[ps2plman] TX: ");
  // print_hex_array(tx_buffer, payload_size + 3);
  // printf("\n");

  // printf("[ps2plman] RX: ");
  // print_hex_array(rx_buffer, payload_size + 3);
  // printf("\n");

  // Automatic 16ms delay after a transmission to allow the controller to register that the packet has completed.
  // This seems to generally match the rate at which the console issues commands (approximately 60 Hz)
  util_delay(16);

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