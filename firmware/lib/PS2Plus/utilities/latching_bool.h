#include <stdbool.h>

/**
 * @brief Boolean value that requires continuous input for a minimum duration or
 *        a minimum number of frames before accepting that input.
 */
typedef struct {
  bool value;
  bool is_actuating;
} latching_bool;

void latching_init(latching_bool *, bool initial_value);
bool latching_update(latching_bool *, bool active);
bool latching_read(latching_bool *);