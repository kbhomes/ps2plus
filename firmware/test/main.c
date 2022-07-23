#include <external/unity/unity.h>
#include <stdio.h>

void setUp() { }
void tearDown() { }

void test_simple(void) {
  TEST_ASSERT_EQUAL(2, 1 + 1);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_simple);
  return UNITY_END();
}