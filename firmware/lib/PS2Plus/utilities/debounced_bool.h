#include <stdbool.h>

/**
 * @brief Boolean value that requires continuous input for a minimum duration or
 *        a minimum number of frames before accepting that input.
 */
typedef struct {
  bool default_value;
  unsigned long minimum_duration;

  unsigned long last_debounce_time;
  bool current_value;
  bool debounced_value;
} debounced_bool;

void debounced_init(debounced_bool *, bool default_value, unsigned long minimum_duration);
bool debounced_update(debounced_bool *, bool value);
bool debounced_read(debounced_bool *);