#include <platform.h>
#include <utilities/latching_bool.h>
#include <unistd.h>
#include <unity.h>

void test_latching_initialization() {
  latching_bool value;
  latching_init(&value, false);

  TEST_ASSERT_FALSE(latching_read(&value)); // Latch is initially false
}

void test_latching_update_flips_value() {
  latching_bool value;
  latching_init(&value, false);
  
  TEST_ASSERT_TRUE(latching_update(&value, true)); // Flips latch to true
}

void test_latching_consecutive_active_updates_retain_value() {
  latching_bool value;
  latching_init(&value, false);
  
  TEST_ASSERT_TRUE(latching_update(&value, true)); // Flips latch to true
  TEST_ASSERT_TRUE(latching_update(&value, true)); // No change (latch is still actuating)
  TEST_ASSERT_TRUE(latching_update(&value, true)); // No change (latch is still actuating)
}

void test_latching_consecutive_updates_flip_value() {
  latching_bool value;
  latching_init(&value, false);
  
  TEST_ASSERT_TRUE(latching_update(&value, true)); // Flips latch to true
  TEST_ASSERT_TRUE(latching_update(&value, false)); // No change (latch is no longer actuating)
  TEST_ASSERT_FALSE(latching_update(&value, true)); // Flips latch to false
}

void test_latching_read_after_update() {
  latching_bool value;
  latching_init(&value, false);
  
  TEST_ASSERT_TRUE(latching_update(&value, true)); // Flips latch to true
  TEST_ASSERT_TRUE(latching_read(&value));
}

int main(void) {
  platform_init();

  UNITY_BEGIN();
  RUN_TEST(test_latching_initialization);
  RUN_TEST(test_latching_update_flips_value);
  RUN_TEST(test_latching_consecutive_active_updates_retain_value);
  RUN_TEST(test_latching_consecutive_updates_flip_value);
  RUN_TEST(test_latching_read_after_update);
  UNITY_END();
}