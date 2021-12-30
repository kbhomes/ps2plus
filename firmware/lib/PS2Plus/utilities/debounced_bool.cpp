#include "debounced_bool.h"

#include "platform.h"

void debounced_init(debounced_bool *debounce, bool default_value, unsigned long minimum_duration) {
  debounce->minimum_duration = minimum_duration;
  debounce->default_value = default_value;
  debounce->current_value = default_value;
  debounce->debounced_value = default_value;
  debounce->last_debounce_time = 0;
}

bool debounced_update(debounced_bool *debounce, bool value) {
  unsigned long current_time = platform_millis();
  unsigned long current_duration = current_time - debounce->last_debounce_time;

  if (value == debounce->default_value) {
    // If the update value is the default state, immediately reset
    debounce->current_value = value;
    debounce->debounced_value = value;
    debounce->last_debounce_time = value;
  } else {
    if (value != debounce->current_value) {
      // If this is the first update, begin the debounce timer
      debounce->current_value = value;
      debounce->last_debounce_time = current_time;
    } else if (current_duration >= debounce->minimum_duration) {
      // Otherwise, if the debounce timer has sufficiently elapsed, then the accept the value
      debounce->debounced_value = value;
    }
  }

  return debounce->debounced_value;
}

bool debounced_read(debounced_bool *debounce) {
  // If a debounce timer is currently running, perform the duration check
  if (debounce->current_value != debounce->debounced_value) {
    unsigned long current_time = platform_millis();
    unsigned long current_duration = current_time - debounce->last_debounce_time;

    if (current_duration >= debounce->minimum_duration) {
      // If the debounce timer has sufficiently elapsed, then the accept the current value
      debounce->debounced_value = debounce->current_value;
    }
  }

  return debounce->debounced_value;
}