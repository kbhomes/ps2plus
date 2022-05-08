#include "digital_button.h"

void digital_button_init(digital_button *button) {
  button->previous = false;
  button->down = false;
}

void digital_button_update(digital_button *button, bool down) {
  button->previous = button->down;
  button->down = down;
}

bool digital_button_is_down(digital_button *button) {
  return button->down;
}

bool digital_button_is_pressed(digital_button *button) {
  return !button->previous && button->down;
}

bool digital_button_is_released(digital_button *button) {
  return button->previous && button->down;
}