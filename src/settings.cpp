#include "pins.h"
#include "config.h"
#include "default.h"

Config config;
Prefs prefs;
State state;

Preferences storage;

void save_config() {

    storage.begin("storage", false); // Open storage on write mode (false)

    // Save every settings division to storage
    storage.putBytes("c", &config, sizeof(Config));
    storage.putBytes("p", &prefs, sizeof(Prefs));
    storage.putBytes("s", &state, sizeof(State));

    storage.end();
}

void load_settings() {

  storage.begin("storage", true); // Open storage on flash memory on only-read mode (true)

  // Load every settings division from storage
  size_t r1 = storage.getBytes("c", &config, sizeof(Config));
  size_t r2 = storage.getBytes("p", &prefs, sizeof(Prefs));
  size_t r3 = storage.getBytes("s", &state, sizeof(State));

  storage.end();

  // Check if every block on memory exists and corresponds to its real memory size
  if (r1 != sizeof(Config) || r2 != sizeof(Prefs) || r3 != sizeof(State)) {
    load_defaults(); save_config(); // If not, load default settings and save them on memory
  }
}

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
