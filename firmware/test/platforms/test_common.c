#include <platforms/platform.h>
#include <unity.h>

void test_platform_timing_millis_monotonically_increases() {
  unsigned long first = platform_timing_millis();
  platform_timing_sleep(10);
  unsigned long second = platform_timing_millis();

  TEST_ASSERT_TRUE_MESSAGE(
      second > first, "Second platform_timing_millis() after sleep should be strictly larger than first platform_timing_millis()");
}

int main(void) {
  platform_init(NULL);
  
  UNITY_BEGIN();
  RUN_TEST(test_platform_timing_millis_monotonically_increases);
  UNITY_END();
}