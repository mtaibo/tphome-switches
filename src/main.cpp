#include <actions.h>
#include <network.h>
#include <buttons.h>
#include <config.h>
#include <pins.h>

int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};

void setup() {

    config_setup();

    // This both lines below activate the pin to allow
    // the blue leds to turn on, otherwise, they'll be blocked
    pinMode(CONFIG_LED, OUTPUT);
    digitalWrite(CONFIG_LED, HIGH);

    // Buttons setup
    buttonTop.setup();
    buttonMid.setup();
    buttonBottom.setup();

    // Loop to turn every output pin to output mode
    // TODO: Turn this output pin configuration into an external pin.begin()
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
        digitalWrite(output_pins[i], LOW);
    }

    network_setup();
}

void loop() {

    // Check if connection is still available
    // and if there are new messages to process
    network_check();

    // Check buttons to see if there was a 
    // physical action on the switch
    buttonTop.check();
    buttonMid.check();
    buttonBottom.check();

    // Update every action that the chip is
    // doing with relays and leds
    updateActions();
}