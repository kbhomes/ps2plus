#include <platforms/platform.h>
#include <utilities/debounced_bool.h>
#include <unistd.h>
#include <unity.h>

void test_debounced_initialization() {
  debounced_bool value;
  debounced_init(&value, true, 10);

  TEST_ASSERT_TRUE(debounced_read(&value));
}

void test_debounced_update_not_immediately_accepted() {
  debounced_bool value;
  debounced_init(&value, false, 10);
  
  TEST_ASSERT_FALSE(debounced_update(&value, true));
}

void test_debounced_update_after_threshold_accepted() {
  debounced_bool value;
  debounced_init(&value, false, 10);
  
  TEST_ASSERT_FALSE(debounced_update(&value, true));
  platform_timing_sleep(25);
  TEST_ASSERT_TRUE(debounced_update(&value, true));
}

void test_debounced_read_after_threshold_updates() {
  debounced_bool value;
  debounced_init(&value, false, 10);
  
  TEST_ASSERT_FALSE(debounced_update(&value, true));
  platform_timing_sleep(25);
  TEST_ASSERT_TRUE(debounced_read(&value));
}

int main(void) {
  platform_init(NULL);

  UNITY_BEGIN();
  RUN_TEST(test_debounced_initialization);
  RUN_TEST(test_debounced_update_not_immediately_accepted);
  RUN_TEST(test_debounced_update_after_threshold_accepted);
  RUN_TEST(test_debounced_read_after_threshold_updates);
  UNITY_END();
}