#include <actions.h> // update_actions();
#include <network.h> // network_setup(); network_check();
#include <buttons.h> // buttonTop/Mid/Bottom.setup()/.check();
#include <config.h> // config_setup();

void setup() {

    config_setup();

    // Buttons setup
    buttonTop.setup();
    buttonMid.setup();
    buttonBottom.setup();

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
    update_actions();
}