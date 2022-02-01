#include "packet.h"
#include "command.h"

void command_packet_initialize(volatile command_packet *packet, command_packet_write_function write_function) {
  packet->id = 0x0;
  packet->command_byte = 0x0;
  packet->packet_index = 0;
  packet->command_index = -3;
  packet->data_index = -2;
  packet->write = write_function;
  packet->ignore = false;
  packet->completed = false;
  packet->processor = NULL;
}

void command_packet_step(volatile command_packet *packet, volatile controller_state *state, uint8_t command_byte) {
  packet->command_byte = command_byte;

  if (!packet->ignore) {
    if (packet->packet_index == 0) {
      packet->write(state->mode);

      // Assuming that we will always be sent a 01h from the console
      if (command_byte != 0x01) {
        packet->ignore = true;
      }
      
    } else if (packet->packet_index == 1) {
      packet->write(0x5A);

      // Second header byte is the command ID
      packet->processor = command_find_processor(command_byte);

      if (packet->processor) {
        packet->id = command_byte;
        ((command_processor *)packet->processor)->initialize(packet, state);
      } else {
        packet->ignore = true;
      }
      
    } else {
      command_result result = ((command_processor *)packet->processor)->process(packet, state);
      if (result == CRCompleted) {
        packet->completed = true;
      } 
    }
  } else if (packet->completed) {
    command_processor *processor = (command_processor *)packet->processor;

    if (processor != NULL && processor->finalize != NULL) {
      processor->finalize(packet, state);
    }
  }
  
  // Diagnostic data
  packet->buf[packet->packet_index & 0xF] = command_byte;
  
  packet->packet_index++;
  packet->command_index++;
  packet->data_index++;
}