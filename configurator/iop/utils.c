#include "utils.h"

int util_thread_id() {
  return GetThreadId();
}

unsigned int util_sleep_callback(void *arg) {
	iWakeupThread(*(int*)arg);
  return 0;
}

void util_sleep(iop_sys_clock_t *timeout, int *thread_id) {
  SetAlarm(timeout, &util_sleep_callback, thread_id);
  SleepThread();
}

void util_delay(u32 ms) {
  DelayThread(ms * 1000);
}

void util_sleep_get_timeout(int ms, iop_sys_clock_t *timeout) {
  USec2SysClock(ms * 100 * 1000, timeout);
};