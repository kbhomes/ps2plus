#include <platform.h>

#ifdef PLATFORM_ARDUINO
#include <Arduino.h>
#define PIN_BUTTON 2
#endif

int main(void) {
  platform_init();

#ifdef PLATFORM_ARDUINO
  pinMode(LED_BUILTIN, OUTPUT);
  
  for (;;) {
    if (digitalRead(PIN_BUTTON) == HIGH) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
#endif

  return 0;
}