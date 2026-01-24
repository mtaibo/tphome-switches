#include <Arduino.h>

#include "config.h"
#include "actions.h"
#include "button.h"
#include "pins.h"

Button btnTop(BTN_TOP);
Button btnMid(BTN_MID);
Button btnBottom(BTN_BOTTOM);

int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};

void setup() {

    config_default();

    btnTop.begin();
    btnMid.begin();
    btnBottom.begin();

    // Loop to turn every output pin to output mode
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
    }
}

void loop() {
    unsigned long durationTop = btnTop.checkPulse();
    if (durationTop > 0) handleButtonAction(BTN_TOP, durationTop);
    
    unsigned long durationMid = btnMid.checkPulse();
    if (durationMid > 0) handleButtonAction(BTN_MID, durationMid);

    unsigned long durationBottom = btnBottom.checkPulse();
    if (durationBottom > 0) handleButtonAction(BTN_BOTTOM, durationBottom);

    updateActions();
}