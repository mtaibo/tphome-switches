#include "leds.h"
#include "settings.h"

void setup() {
    Settings::setup();
    Hardware::setup();

    Leds::set(LED_TOP, Leds::RED, Leds::MEDIUM, 0, 1000);
    Leds::set(LED_MID, Leds::RED, Leds::MEDIUM, 0, 1000);
    Leds::set(LED_BTM, Leds::RED, Leds::MEDIUM, 0, 1000);

    Leds::set(LED_GREEN, Leds::BLINK, Leds::MEDIUM, 0, 1000);
}

void loop() {
    Leds::update();
}