#include <shared/primitive_data.h>
#include <string.h>

#include "suite.h"

primitive_data data;

// Useful static array constant
static const uint8_t array[8] = { 0xFF };

void test_primitive_data_initialize_boolean() {
  primitive_data_initialize_boolean(&data, true);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Boolean, data.type, "Boolean.type");
  TEST_ASSERT_EQUAL_MESSAGE(true, data.boolean, "Boolean.value");
}

void test_primitive_data_initialize_uint8() {
  primitive_data_initialize_uint8(&data, 1);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint8, data.type, "Uint8.type");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, data.uint8, "Uint8.value");
}

void test_primitive_data_initialize_uint16() {
  primitive_data_initialize_uint16(&data, 2);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint16, data.type, "Uint16.type");
  TEST_ASSERT_EQUAL_UINT16_MESSAGE(2, data.uint16, "Uint16.value");
}

void test_primitive_data_initialize_uint32() {
  primitive_data_initialize_uint32(&data, 3);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint32, data.type, "Uint32.type");
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(3, data.uint32, "Uint32.value");
}

void test_primitive_data_initialize_uint64() {
  primitive_data_initialize_uint64(&data, 4);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint64, data.type, "Uint64.type");
  TEST_ASSERT_EQUAL_UINT64_MESSAGE(4, data.uint64, "Uint64.value");
}

void test_primitive_data_initialize_array() {
  primitive_data_initialize_array(&data, array, sizeof(array));
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Array, data.type, "Array.type");
  TEST_ASSERT_EQUAL_MESSAGE(sizeof(array), data.array.length, "Array.length");
  TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(array, data.array.data, sizeof(array), "Array.data");
}

void test_primitive_data_length() {
  primitive_data_initialize_boolean(&data, true);
  TEST_ASSERT_EQUAL_MESSAGE(1 + sizeof(bool), primitive_data_length(&data), "Boolean");

  primitive_data_initialize_uint8(&data, 1);
  TEST_ASSERT_EQUAL_MESSAGE(1 + sizeof(uint8_t), primitive_data_length(&data), "Uint8");

  primitive_data_initialize_uint16(&data, 2);
  TEST_ASSERT_EQUAL_MESSAGE(1 + sizeof(uint16_t), primitive_data_length(&data), "Uint16");

  primitive_data_initialize_uint32(&data, 3);
  TEST_ASSERT_EQUAL_MESSAGE(1 + sizeof(uint32_t), primitive_data_length(&data), "Uint32");

  primitive_data_initialize_uint64(&data, 4);
  TEST_ASSERT_EQUAL_MESSAGE(1 + sizeof(uint64_t), primitive_data_length(&data), "Uint64");

  primitive_data_initialize_array(&data, array, sizeof(array));
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Array, data.type, "Array.type");
  TEST_ASSERT_EQUAL_MESSAGE(1 + sizeof(uint8_t) + sizeof(array), primitive_data_length(&data), "Array.value");

  // Invalid types always return a length of 1
  data.type = PDT_Unknown;
  TEST_ASSERT_EQUAL(1, primitive_data_length(&data));
}

void test_primitive_data_serialize() {
  static uint8_t out[128] = { 0 };

  memset(out, 0, sizeof(out));
  primitive_data_initialize_boolean(&data, true);
  primitive_data_serialize(&data, out);
  const uint8_t expected_boolean[] = { PDT_Boolean, 0xFF };
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_boolean, out, sizeof(expected_boolean), "Boolean");

  memset(out, 0, sizeof(out));
  primitive_data_initialize_uint8(&data, 1);
  primitive_data_serialize(&data, out);
  const uint8_t expected_uint8[] = { PDT_Uint8, 1 };
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_uint8, out, sizeof(expected_uint8), "Uint8");

  memset(out, 0, sizeof(out));
  primitive_data_initialize_uint16(&data, 2);
  primitive_data_serialize(&data, out);
  const uint8_t expected_uint16[] = { PDT_Uint16, 2, 0 };
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_uint16, out, sizeof(expected_uint16), "Uint16");

  memset(out, 0, sizeof(out));
  primitive_data_initialize_uint32(&data, 3);
  primitive_data_serialize(&data, out);
  const uint8_t expected_uint32[] = { PDT_Uint32, 3, 0, 0, 0 };
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_uint32, out, sizeof(expected_uint32), "Uint32");

  memset(out, 0, sizeof(out));
  primitive_data_initialize_uint64(&data, 4);
  primitive_data_serialize(&data, out);
  const uint8_t expected_uint64[] = { PDT_Uint64, 4, 0, 0, 0, 0, 0, 0, 0 };
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_uint64, out, sizeof(expected_uint64), "Uint64");

  memset(out, 0, sizeof(out));
  primitive_data_initialize_array(&data, array, sizeof(array));
  primitive_data_serialize(&data, out);
  const uint8_t expected_array[2 + sizeof(array)] = { PDT_Array, sizeof(array) };
  memcpy(expected_array + 2, array, sizeof(array));
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_array, out, sizeof(expected_array), "Array");

  memset(out, 0, sizeof(out));
  data.type = PDT_Unknown;
  primitive_data_serialize(&data, out);
  const uint8_t expected_unknown[] = { PDT_Unknown };
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_unknown, out, sizeof(expected_unknown), "Unknown");
}

void test_primitive_data_deserialize() {
  const uint8_t in_boolean[] = { PDT_Boolean, 0xFF };
  primitive_data_deserialize(&data, in_boolean);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Boolean, data.type, "Boolean.type");
  TEST_ASSERT_EQUAL_MESSAGE(true, data.boolean, "Boolean.value");

  const uint8_t in_uint8[] = { PDT_Uint8, 1 };
  primitive_data_deserialize(&data, in_uint8);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint8, data.type, "Uint8.type");
  TEST_ASSERT_EQUAL_MESSAGE(1, data.uint8, "Uint8.value");

  const uint8_t in_uint16[] = { PDT_Uint16, 2, 0 };
  primitive_data_deserialize(&data, in_uint16);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint16, data.type, "Uint16.type");
  TEST_ASSERT_EQUAL_MESSAGE(2, data.uint16, "Uint16.value");

  const uint8_t in_uint32[] = { PDT_Uint32, 3, 0, 0, 0 };
  primitive_data_deserialize(&data, in_uint32);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint32, data.type, "Uint32.type");
  TEST_ASSERT_EQUAL_MESSAGE(3, data.uint32, "Uint32.value");

  const uint8_t in_uint64[] = { PDT_Uint64, 4, 0, 0, 0, 0, 0, 0, 0 };
  primitive_data_deserialize(&data, in_uint64);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Uint64, data.type, "Uint64.type");
  TEST_ASSERT_EQUAL_MESSAGE(4, data.uint64, "Uint64.value");

  const uint8_t in_array[2 + sizeof(array)] = { PDT_Array, sizeof(array) };
  memcpy(in_array + 2, array, sizeof(array));
  primitive_data_deserialize(&data, in_array);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Array, data.type, "Array.type");
  TEST_ASSERT_EQUAL_MESSAGE(sizeof(array), data.array.length, "Array.length");
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(array, data.array.data, sizeof(array), "Array.data");

  const uint8_t in_unknown[] = { PDT_Unknown };
  primitive_data_deserialize(&data, in_unknown);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Unknown, data.type, "Unknown.type");

  // Invalid types are deserialized as "unknown"
  const uint8_t in_invalid[] = { 0x55, 0x66, 0x77 };
  primitive_data_deserialize(&data, in_invalid);
  TEST_ASSERT_EQUAL_MESSAGE(PDT_Unknown, data.type, "Unknown.type (from invalid)");
}

void test_primitive_data_equals() {
  primitive_data bool_a, bool_b, bool_c;
  primitive_data_initialize_boolean(&bool_a, false);
  primitive_data_initialize_boolean(&bool_b, false);
  primitive_data_initialize_boolean(&bool_c, true);
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&bool_a, &bool_b), "Expected false = false");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&bool_a, &bool_c), "Expected false != true");
  
  primitive_data uint8_a, uint8_b, uint8_c;
  primitive_data_initialize_uint8(&uint8_a, 1);
  primitive_data_initialize_uint8(&uint8_b, 1);
  primitive_data_initialize_uint8(&uint8_c, 2);
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&uint8_a, &uint8_b), "Expected 1 = 1");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&uint8_a, &uint8_c), "Expected 1 != 2");
  
  primitive_data uint16_a, uint16_b, uint16_c;
  primitive_data_initialize_uint16(&uint16_a, 1);
  primitive_data_initialize_uint16(&uint16_b, 1);
  primitive_data_initialize_uint16(&uint16_c, 2);
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&uint16_a, &uint16_b), "Expected 1 = 1");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&uint16_a, &uint16_c), "Expected 1 != 2");
  
  primitive_data uint32_a, uint32_b, uint32_c;
  primitive_data_initialize_uint32(&uint32_a, 1);
  primitive_data_initialize_uint32(&uint32_b, 1);
  primitive_data_initialize_uint32(&uint32_c, 2);
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&uint32_a, &uint32_b), "Expected 1 = 1");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&uint32_a, &uint32_c), "Expected 1 != 2");
  
  primitive_data uint64_a, uint64_b, uint64_c;
  primitive_data_initialize_uint64(&uint64_a, 1);
  primitive_data_initialize_uint64(&uint64_b, 1);
  primitive_data_initialize_uint64(&uint64_c, 2);
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&uint64_a, &uint64_b), "Expected 1 = 1");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&uint64_a, &uint64_c), "Expected 1 != 2");

  primitive_data array_short_a, array_short_b, array_short_c, array_long;
  const uint8_t short_a_data[] = { 1, 2, 3 };
  const uint8_t short_b_data[] = { 1, 2, 3 };
  const uint8_t short_c_data[] = { 40, 50, 60 };
  const uint8_t long_data[] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
  primitive_data_initialize_array(&array_short_a, short_a_data, sizeof(short_a_data));
  primitive_data_initialize_array(&array_short_b, short_b_data, sizeof(short_b_data));
  primitive_data_initialize_array(&array_short_c, short_c_data, sizeof(short_c_data));
  primitive_data_initialize_array(&array_long, long_data, sizeof(long_data));
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&array_short_a, &array_short_b), "Expected ShortA[] = ShortB[]");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&array_short_a, &array_short_c), "Expected ShortA[] != ShortC[]");
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&array_short_a, &array_long), "Expected ShortA[] != Long[]");

  primitive_data unknown_a = { .type = PDT_Unknown }, unknown_b = { .type = PDT_Unknown};
  TEST_ASSERT_TRUE_MESSAGE(primitive_data_equals(&unknown_a, &unknown_b), "Expected Unknown = Unknown");

  // Invalid data types are always unequal, even if they are literally the same
  primitive_data invalid_data = { .type = 0x55 };
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&invalid_data, &invalid_data), "Expected invalid != invalid");

  // Different type data are different even if their underlying values are the same
  TEST_ASSERT_FALSE_MESSAGE(primitive_data_equals(&uint8_a, &uint16_a), "Expected 1 (uint8) != 1 (uint16)");
}

int suite_shared_primitive_data() {
  UNITY_BEGIN("Shared - Primitive Data");
  RUN_TEST(test_primitive_data_initialize_boolean);
  RUN_TEST(test_primitive_data_initialize_uint8);
  RUN_TEST(test_primitive_data_initialize_uint16);
  RUN_TEST(test_primitive_data_initialize_uint32);
  RUN_TEST(test_primitive_data_initialize_uint64);
  RUN_TEST(test_primitive_data_initialize_array);
  RUN_TEST(test_primitive_data_length);
  RUN_TEST(test_primitive_data_serialize);
  RUN_TEST(test_primitive_data_deserialize);
  RUN_TEST(test_primitive_data_equals);
  return UNITY_END();
}