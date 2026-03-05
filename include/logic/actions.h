#ifndef ACTIONS_H
#define ACTIONS_H

#include "buttons.h"
#include "position.h"

namespace Actions {
    inline void check() {
        
        if (auto action = Buttons::getAction(BTN_TOP)) {
            //if (action == Buttons::SHORT) Relays::moveUp();
            //else                          Relays::fullUp();
            if (action == Buttons::SHORT) Leds::set(LED_TOP, Leds::BLINK, Leds::SLOW, 10);
        }

        /*
        if (auto action = Buttons::getAction(BTN_MID)) {
            if (action == Buttons::SHORT) Relays::stop();
        } */
    }
}

#endif