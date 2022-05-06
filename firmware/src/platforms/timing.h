#ifndef PLATFORMS_TIMING_H
#define PLATFORMS_TIMING_H

/**
 * @brief Returns a millisecond-resolution system time
 */
unsigned long platform_timing_millis(void);

/**
 * @brief Attempts to sleep for the provided number of milliseconds 
 */
void platform_timing_sleep(unsigned int millis);

#endif /* PLATFORMS_TIMING_H */