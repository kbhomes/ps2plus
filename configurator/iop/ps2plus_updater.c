#include "irx_imports.h"
#include "ps2plus_updater.h"

#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define RX_HEADER_LENGTH 3
#define TX_PLACEHOLDER 0x00

IRX_ID("ps2plupd", 1, 1);

extern struct irx_export_table _exp_ps2plupd;

// RPC thread and server
static s32 rpc_thread_id;
static SifRpcDataQueue_t rpc_data_queue;
static SifRpcServerData_t rpc_server_data;
static u32 rpc_server_buffer[32];

// SIO2 transfer data
static sio2_transfer_data_t pad_transfer_data;
static u8 sio2_in_buffer[256];
static u8 sio2_out_buffer[256];

// 48 MHz clock divided by 192 (C0h) results in a 250 KHz clock signal
static const int clock_divider = 0xC0;
static u8 tx_buffer[128];
static u8 rx_buffer[128];

static u8 poll_tx[] = { 0x01, 0x42, 0x00, 0x00, 0x00 };
static u8 command_tx[] = { 0x01, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00 };

static void initSio2Transfer() {
    // Initialize all control registers
    for (int i = 0; i < 4; i++) {
        pad_transfer_data.port_ctrl1[i] = 0;
        pad_transfer_data.port_ctrl2[i] = 0;
    }

    // TODO: Assuming port 0 for now 
    pad_transfer_data.port_ctrl1[0] = 
        PCTRL0_ATT_LOW_PER(0x05) |
        PCTRL0_ATT_MIN_HIGH_PER(0x05) |
        PCTRL0_BAUD0_DIV(clock_divider) |
        PCTRL0_BAUD1_DIV(clock_divider);
    
    pad_transfer_data.port_ctrl2[0] = 
        PCTRL1_ACK_TIMEOUT_PER(0x12C) |
        PCTRL1_INTER_BYTE_PER(0) |
        PCTRL1_UNK24(0) |
        PCTRL1_IF_MODE_SPI_DIFF(0);

    pad_transfer_data.in = NULL;
    pad_transfer_data.in_size = 0;
    pad_transfer_data.in_dma.addr = NULL;
    pad_transfer_data.in_dma.size = 0;
    pad_transfer_data.in_dma.count = 0;

    pad_transfer_data.out = NULL;
    pad_transfer_data.out_size = 0;
    pad_transfer_data.out_dma.addr = NULL;
    pad_transfer_data.out_dma.size = 0;
    pad_transfer_data.out_dma.count = 0;

    // Set all port control bits
    for (int i = 0; i < 4; i++) {
        inl_sio2_portN_ctrl1_set(i, pad_transfer_data.port_ctrl1[i]);
        inl_sio2_portN_ctrl2_set(i, pad_transfer_data.port_ctrl2[i]);
    }
}

static void transferSio2Data(u8 *tx_buffer, size_t tx_length, u8 *rx_buffer, size_t rx_length) {
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
        inl_sio2_data_out(*tx_buffer++);
    }

    // Start the SIO2 transfer
    inl_sio2_ctrl_set(inl_sio2_ctrl_get() | 1);

    // Wait for completion
    while ((inl_sio2_stat6c_get() & (1 << 12)) == 0);

    // Read from the SIO2 RX shift register into the receive buffer
    while (rx_length--) {
        *rx_buffer++ = inl_sio2_data_in();
    }
}

void print_hex_array(const u8 *array, size_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%02X ", array[i]);
  }
}

static void padConnect() {
    size_t command_size = sizeof(poll_tx);
    memcpy(tx_buffer, poll_tx, command_size);
    memset(rx_buffer, 0, command_size);

    initSio2Transfer();
    transferSio2Data(tx_buffer, command_size, rx_buffer, command_size);

    printf("[ps2plupd] TX: ");
    print_hex_array(tx_buffer, command_size);
    printf("\n");
    
    printf("[ps2plupd] RX: ");
    print_hex_array(rx_buffer, command_size);
    printf("\n");
}

static uint8_t tx_get_version[] = { 0x01, 0x70, 0x00, TX_PLACEHOLDER, 0x00, 0x00 };
static uint8_t rx_get_version[] = { 0xFF, 0x41, 0x5A, 0x00, PDT_Array, 0x06, 'H', 'e', 'l', 'l', 'o', '!' };
// static uint8_t rx_get_version[] = { 0xFF, 0x41, 0x5A, 0x00, PDT_Uint16, 0x32, 0xAB };

void transferSio2Data_mock(u8 *tx_buffer, u8 *rx_buffer, size_t command_size, u8 *rx_mock) {
    if (rx_mock) {
        memcpy(rx_buffer, rx_mock, command_size);
    } else {
        transferSio2Data(tx_buffer, command_size, rx_buffer, command_size);
    }
}

static void ps2plus_get_version(ps2plus_updater_rpc_packet *packet) {
    // First pass to get the size of the version payload
    {
        size_t command_size = sizeof(tx_get_version);
        memset(rx_buffer, 0, sizeof(rx_buffer));
        memset(tx_buffer, 0, sizeof(tx_buffer));
        memcpy(tx_buffer, command_tx, command_size);
        tx_buffer[3] = packet->command_get_version.version_id;

        initSio2Transfer();
        transferSio2Data_mock(tx_buffer, rx_buffer, command_size, rx_get_version);

        printf("[ps2plupd] TX: ");
        print_hex_array(tx_buffer, command_size);
        printf("\n");
        
        printf("[ps2plupd] RX: ");
        print_hex_array(rx_buffer, command_size);
        printf("\n");

        packet->command_get_version.version_response.type = rx_buffer[RX_HEADER_LENGTH + 1];
        packet->command_get_version.version_response.array.length = rx_buffer[RX_HEADER_LENGTH + 2];
        printf("[ps2plupd] Type=%d Next=%d\n", packet->command_get_version.version_response.type, packet->command_get_version.version_response.array.length);
    }

    // Second pass to get the real payload
    {
        size_t primitive_size = primitive_data_length(&packet->command_get_version.version_response);
        size_t command_size = sizeof(tx_get_version);
        memset(rx_buffer, 0, sizeof(rx_buffer));
        memset(tx_buffer, 0, sizeof(tx_buffer));
        memcpy(tx_buffer, command_tx, command_size);
        tx_buffer[3] = packet->command_get_version.version_id;

        initSio2Transfer();
        transferSio2Data_mock(tx_buffer, rx_buffer, command_size - 2 + primitive_size, rx_get_version);

        printf("[ps2plupd] TX: ");
        print_hex_array(tx_buffer, command_size - 2 + primitive_size);
        printf("\n");
        
        printf("[ps2plupd] RX: ");
        print_hex_array(rx_buffer, command_size - 2 + primitive_size);
        printf("\n");

        primitive_data_deserialize(&packet->command_get_version.version_response, rx_buffer + RX_HEADER_LENGTH + 1);
        
        // Handle string/array responses
        if (packet->command_get_version.version_response.type == PDT_Array) {
            // Copy the array into the response buffer
            memcpy(
                packet->command_get_version.version_buffer, 
                packet->command_get_version.version_response.array.data, 
                MIN(sizeof(packet->command_get_version.version_buffer), packet->command_get_version.version_response.array.length)
            );

            packet->command_get_version.version_response.array.data = NULL;
        }
    }
}

static void *rpcServer(int fno, void *buffer, int length) {
    ps2plus_updater_rpc_packet *packet = (ps2plus_updater_rpc_packet *)buffer;
    ps2plus_updater_rpc_command command = packet->command;

    switch (command) {
        case PS2Plus_Init:
            printf("[ps2plupd] Command: PS2Plus_Init\n");
            padConnect();
            return buffer;

        case PS2Plus_GetVersion:
            printf("[ps2plupd] Command: PS2Plus_GetVersion\n");
            ps2plus_get_version(packet);
            return buffer;

        case PS2Plus_GetConfiguration:
            printf("[ps2plupd] Command: PS2Plus_GetConfiguration\n");
            return buffer;

        case PS2Plus_SetConfiguration:
            printf("[ps2plupd] Command: PS2Plus_SetConfiguration\n");
            return buffer;

        case PS2Plus_DisableEnableConfiguration:
            printf("[ps2plupd] Command: PS2Plus_DisableEnableConfiguration\n");
            return buffer;

        case PS2Plus_RestoreConfigurationDefaults:
            printf("[ps2plupd] Command: PS2Plus_RestoreConfigurationDefaults\n");
            return buffer;

        default:
            printf("[ps2plupd] Command: Unknown (%d)\n", command);
            break;
    }

    return buffer;
}

static void rpcThread(void *arg) {
    if (sceSifCheckInit() == 0) {
        printf("[ps2plupd] SIF not initialized\n");
        sceSifInit();
    }

    sceSifInitRpc(0);
    sceSifSetRpcQueue(&rpc_data_queue, GetThreadId());
    sceSifRegisterRpc(&rpc_server_data, PS2PLUPD_RPC_BIND_ID, &rpcServer, rpc_server_buffer, NULL, NULL, &rpc_data_queue);
    sceSifRpcLoop(&rpc_data_queue);
}

void ps2PlusUpdaterInit(void) {
    iop_thread_t rpc_thread;

    // RPC Server
    rpc_thread.attr = TH_C;
    rpc_thread.thread = &rpcThread;
    rpc_thread.stacksize = 0x800;
    rpc_thread.priority = PS2PLUPD_THREAD_PRIO;
    rpc_thread_id = CreateThread(&rpc_thread);
    StartThread(rpc_thread_id, NULL);
}

int _start(int argc, char * argv[]) {
    if (RegisterLibraryEntries(&_exp_ps2plupd) != 0)
        return MODULE_NO_RESIDENT_END;

    ps2PlusUpdaterInit();

    return MODULE_RESIDENT_END;
}