#include "latching_bool.h"

void latching_init(latching_bool *latch, bool initial_value) {
  latch->is_actuating = false;
  latch->value = initial_value;
}

bool latching_update(latching_bool *latch, bool active) {
  // If the latch is being *newly* actuated, flip its value
  if (active && !latch->is_actuating) {
    latch->value = !latch->value; 
  }

  // Track whether the latch is currently actuating
  latch->is_actuating = active;
  
  return latch->value;
}

bool latching_read(latching_bool *latch) {
  return latch->value;
}