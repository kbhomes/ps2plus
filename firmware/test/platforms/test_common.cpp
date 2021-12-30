#include <platform.h>
#include <unity.h>

void test_platform_millis_monotonically_increases() {
  unsigned long first = platform_millis();
  platform_sleep(10);
  unsigned long second = platform_millis();

  TEST_ASSERT_TRUE_MESSAGE(
      second > first, "Second platform_millis() after sleep should be strictly larger than first platform_millis()");
}

int main(void) {
  platform_init();
  
  UNITY_BEGIN();
  RUN_TEST(test_platform_millis_monotonically_increases);
  UNITY_END();
}