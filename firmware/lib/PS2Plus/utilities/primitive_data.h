#ifndef UTILITIES_PRIMITIVE_DATA_H
#define UTILITIES_PRIMITIVE_DATA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum primitive_data_type {
  PDT_Boolean = 0x01,
  PDT_Uint8 = 0x02,
  PDT_Uint16 = 0x03,
  PDT_Uint32 = 0x04,
  PDT_Array = 0x05,
  PDT_Unknown = 0xFF
} primitive_data_type;

typedef struct primitive_data_array {
  uint8_t length;
  uint8_t *data;
} primitive_data_array;

typedef struct primitive_data {
  uint8_t type; // `primitive_data_type` enum
  union data {
    bool boolean;
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    primitive_data_array array;
  } data;
} primitive_data;

void primitive_data_initialize_boolean(primitive_data *pd, bool value);
void primitive_data_initialize_uint8(primitive_data *pd, uint8_t value);
void primitive_data_initialize_uint16(primitive_data *pd, uint16_t value);
void primitive_data_initialize_uint32(primitive_data *pd, uint32_t value);
void primitive_data_initialize_array(primitive_data *pd, uint8_t *value, uint8_t length);
size_t primitive_data_length(primitive_data *pd);
void primitive_data_serialize(primitive_data *pd, uint8_t *out);
void primitive_data_deserialize(primitive_data *pd, uint8_t *in);

#endif /* UTILITIES_PRIMITIVE_DATA_H */