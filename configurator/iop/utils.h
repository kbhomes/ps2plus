#ifndef IOP_UTILS_H
#define IOP_UTILS_H

#include "irx_imports.h"

int util_thread_id();
unsigned int util_sleep_callback(void *arg);
void util_sleep(iop_sys_clock_t *timeout, int *thread_id);
void util_sleep_get_timeout(int ms, iop_sys_clock_t *timeout);

void util_delay(u32 ms);

#endif /* IOP_UTILS_H */