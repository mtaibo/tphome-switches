#include "leds.h"
#include "settings.h"

void setup() {
    Settings::setup();
    Hardware::setup();
}

void loop() {
    Leds::update();
}
