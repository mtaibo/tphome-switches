#include <actions.h>
#include <buttons.h>
#include <config.h>
#include <wifi.h>
#include <pins.h>

int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};

void setup() {

    config_init();

    // Buttons setup
    buttonTop.setup();
    buttonMid.setup();
    buttonBottom.setup();

    // Loop to turn every output pin to output mode
    // TODO: Turn this output pin configuration into an external pin.begin()
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
    }

    // 
    setup_wifi();
    setup_mqtt();
}

void loop() {

    // Update wifi connection and check if
    // there was a client interaction
    if (!client.connected()) reconnect();
    client.loop(); 

    // Check buttons to see if there was a 
    // physical action on the switch
    buttonTop.check();
    buttonMid.check();
    buttonBottom.check();

    // Update every action that the chip is
    // doing with relays and leds
    updateActions();
}