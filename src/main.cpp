#include "settings.h"
#include "actions.h"

void setup() {
    Settings::setup();
    Hardware::setup();
}

void loop() {

    Actions::check();

    Leds::update();
    Relays:: update();
    Buttons::update();
    Position::update();
}