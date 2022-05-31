#include "primitive_data.h"

#include <string.h>

const primitive_data primitive_data_unknown = { .type = PDT_Unknown };
const primitive_data primitive_data_boolean_default = { .type = PDT_Boolean, .boolean = false };
const primitive_data primitive_data_uint8_default = { .type = PDT_Uint8, .uint8 = 0 };
const primitive_data primitive_data_uint16_default = { .type = PDT_Uint16, .uint16 = 0 };
const primitive_data primitive_data_uint32_default = { .type = PDT_Uint32, .uint32 = 0 };
const primitive_data primitive_data_uint64_default = { .type = PDT_Uint64, .uint64 = 0 };
const primitive_data primitive_data_array_default = { .type = PDT_Array };

void primitive_data_initialize_boolean(primitive_data *pd, bool value) {
  pd->type = PDT_Boolean;
  pd->boolean = value;
}

void primitive_data_initialize_uint8(primitive_data *pd, uint8_t value) {
  pd->type = PDT_Uint8;
  pd->uint8 = value;
}

void primitive_data_initialize_uint16(primitive_data *pd, uint16_t value) {
  pd->type = PDT_Uint16;
  pd->uint16 = value;
}

void primitive_data_initialize_uint32(primitive_data *pd, uint32_t value) {
  pd->type = PDT_Uint32;
  pd->uint32 = value;
}

void primitive_data_initialize_uint64(primitive_data *pd, uint64_t value) {
  pd->type = PDT_Uint64;
  pd->uint64 = value;
}

void primitive_data_initialize_array(primitive_data *pd, uint8_t *value, uint8_t length) {
  pd->type = PDT_Array;
  pd->array.length = length;
  pd->array.data = value;
}

size_t primitive_data_length(primitive_data *pd) {
  switch (pd->type) {
    case PDT_Boolean: return 2;
    case PDT_Uint8: return 2;
    case PDT_Uint16: return 3;
    case PDT_Uint32: return 5;
    case PDT_Uint64: return 9;
    case PDT_Array: return 2 + pd->array.length;
    default: return 1;
  }
}

void primitive_data_serialize(primitive_data *pd, uint8_t *out) {
  out[0] = pd->type;

  switch (pd->type) {
    case PDT_Boolean:
      out[1] = pd->boolean ? 0xFF : 0x00;
      return;
    
    case PDT_Uint8:
      out[1] = pd->uint8;
      return;
      
    case PDT_Uint16:
      out[1] = (pd->uint16 & 0xFF);
      out[2] = (pd->uint16 >> 8) & 0xFF;
      return;
      
    case PDT_Uint32:
      out[1] = (pd->uint32 & 0xFF);
      out[2] = (pd->uint32 >> 8) & 0xFF;
      out[3] = (pd->uint32 >> 16) & 0xFF;
      out[4] = (pd->uint32 >> 24) & 0xFF;
      return;
      
    case PDT_Uint64:
      out[1] = (pd->uint64 & 0xFF);
      out[2] = (pd->uint64 >> 8) & 0xFF;
      out[3] = (pd->uint64 >> 16) & 0xFF;
      out[4] = (pd->uint64 >> 24) & 0xFF;
      out[5] = (pd->uint64 >> 32) & 0xFF;
      out[6] = (pd->uint64 >> 40) & 0xFF;
      out[7] = (pd->uint64 >> 48) & 0xFF;
      out[8] = (pd->uint64 >> 56) & 0xFF;
      return;

    case PDT_Array:
      out[1] = pd->array.length;
      memcpy(out + 2, pd->array.data, pd->array.length);
      return;

    case PDT_Unknown:
    default:
      out[1] = 0xFF;
      return;
  }
}

void primitive_data_deserialize(primitive_data *pd, uint8_t *in) {
  pd->type = in[0];

  switch (pd->type) {
    case PDT_Boolean:
      pd->boolean = in[1] != 0x00;
      return;
    
    case PDT_Uint8:
      pd->uint8 = in[1];
      return;
      
    case PDT_Uint16:
      pd->uint16 = in[1] | ((uint16_t)in[2] << 8);
      return;
      
    case PDT_Uint32:
      pd->uint32 = in[1] | ((uint32_t)in[2] << 8) | ((uint32_t)in[3] << 16) | ((uint32_t)in[4] << 24);
      return;
      
    case PDT_Uint64:
      pd->uint64 = (
        in[1] | ((uint64_t)in[2] << 8) | 
        ((uint64_t)in[3] << 16) | ((uint64_t)in[4] << 24) | 
        ((uint64_t)in[5] << 32) | ((uint64_t)in[6] << 40) | 
        ((uint64_t)in[7] << 48) | ((uint64_t)in[8] << 56)
      );
      return;

    case PDT_Array:
      pd->array.length = in[1];
      pd->array.data = in + 2;
      return;

    case PDT_Unknown:
    default:
      return;
  }
}

bool primitive_data_equals(primitive_data *a, primitive_data *b) {
  if (a->type != b->type) {
    return false;
  }

  switch (a->type) {
    case PDT_Boolean: 
      return a->boolean == b->boolean;

    case PDT_Uint8: 
      return a->uint8 == b->uint8;

    case PDT_Uint16: 
      return a->uint16 == b->uint16;

    case PDT_Uint32: 
      return a->uint32 == b->uint32;

    case PDT_Uint64: 
      return a->uint64 == b->uint64;

    case PDT_Array: 
      return (a->array.length == b->array.length && (
        (a->array.data == NULL && b->array.data == NULL) || 
        memcmp(a->array.data, b->array.data, a->array.length) == 0
      ));

    case PDT_Unknown:
      return true;
  }

  return false;
}