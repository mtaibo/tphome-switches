#include "pins.h"
#include "config.h"
#include "default.h"

Config config;
Prefs prefs;
State state;

Preferences storage;

void load_settings() {}
void save_config() {}
void reset_memory() {}

void config_setup() {

  // Send current through the pin that provides it to blue leds
  pinMode(PIN_LED_CONF, OUTPUT);
  digitalWrite(PIN_LED_CONF, HIGH);

  // Turn every other pin to output to allow HIGH and LOW state, and LOW to begin with every pin off
  for (uint8_t pin : {PIN_LED_TOP, PIN_LED_MID, PIN_LED_BOTTOM, PIN_LED_GREEN, PIN_RELAY_UP, PIN_RELAY_DOWN}) {
    pinMode(pin, OUTPUT); digitalWrite(pin, LOW);
  }

  load_config();
}
