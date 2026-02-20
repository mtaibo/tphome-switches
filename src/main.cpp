#include "actions.h"
#include "network.h"
#include "buttons.h"
#include "config.h"

void setup() {

    config_setup();
    network_setup();

    buttonTop.setup();
    buttonMid.setup();
    buttonBottom.setup();
}

void loop() {

    update_actions();
    network_check();
 
    buttonTop.check();
    buttonMid.check();
    buttonBottom.check();

    delay(10);
}
