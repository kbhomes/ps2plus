#ifndef COMMANDS_PACKET_H
#define COMMANDS_PACKET_H

#include <controller/state.h>
#include <platforms/platform.h>

typedef void (*command_packet_write_function)(uint8_t data);

/**
 * @brief Stateful structure representing the current command packet
 * 
 * This packet structure is updated as data is read from/written to the console. The `packet_index` field
 * is updated each time a byte is read to indicate the current location within the packet. The other indices
 * are for convenience and code clarity within a command processor to skip the packet header. The `command_index` 
 * is the index of the most recently read byte (the first payload byte is index 0). The `data_index` is the index
 * of the *to-be-written* byte; an index of 0 means the next byte written will be the first byte of the payload.
 * 
 * @example
 *   Label:          (header)   |  (payload)
 *   TX:            01  42  00  |   00  00
 *   RX:            FF  41  5A  |   FF  FF
 *   packet_index:   0   1   2  |    3   4
 *   command_index: -3  -2  -1  |    0   1
 *   data_index:    -2  -1   0  |    1   2
 */
typedef struct command_packet {
  uint8_t buf[16];

  /**
   * @brief Command ID for this packet (e.g.: 42h for the main polling command)
   */
  uint8_t id;

  /**
   * @brief Most recently read byte from the console
   */
  uint8_t command_byte;

  /**
   * @brief Index of the most recently read/received byte in the command packet, including the 3-byte header
   */
  int packet_index;

  /**
   * @brief Index of the most recently read COMMAND byte in the command packet, not including the 3-byte header
   */
  int command_index;

  /**
   * @brief Index of the to-be-written DATA byte in the command packet, not including the 3-byte header
   */
  int data_index;

  /**
   * @brief Write function to send a byte back to the console
   */
  command_packet_write_function write;

  /**
   * @brief True if this packet should be ignored for the remainder of its lifetime due to invalid data
   */
  bool ignore;

  /**
   * @brief True if this packet has written its final byte (though it may still read the final incoming byte)
   */
  bool completed;

  /**
   * @brief Command processor that will handle this packet
   */
  void *processor;
} command_packet;

void command_packet_initialize(volatile command_packet *packet, command_packet_write_function write_function);
void command_packet_step(volatile command_packet *packet, controller_state *state, uint8_t command_byte);

#endif /* COMMANDS_PACKET_H */