#include <platform.h>

#ifdef PLATFORM_ARDUINO
#include <Arduino.h>
#define PIN_BUTTON 2
#endif

int main(void) {
  platform_init();

#ifdef PLATFORM_ARDUINO
  Serial.begin(9600);

  for (;;) {
    if (digitalRead(PIN_BUTTON) == HIGH) {
      platform_spi_write(0x32);
    } else {
      platform_spi_read();
    }
  }
#endif

  return 0;
}