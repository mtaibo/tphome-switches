#ifndef MODE_H
#define MODE_H

#include "leds.h"

namespace Mode {

    enum Value {
        NORMAL,
        CONNECTION
    };

    inline Value current = NORMAL;
    inline bool is(Value mode) { return current == mode; }

    inline void set(Value mode) { 
        
        if (mode == current) return;

        if (mode == NORMAL) {
            Leds::set(Pins::LED_TOP, Leds::OFF);
            Leds::set(Pins::LED_MID, Leds::OFF);
            Leds::set(Pins::LED_BTM, Leds::OFF);
            Leds::set(Pins::LED_GREEN, Leds::OFF);
        }

        else if (mode == CONNECTION) {
            Leds::set(Pins::LED_TOP, Leds::BLINK);
            Leds::set(Pins::LED_MID, Leds::ON);
            Leds::set(Pins::LED_BTM, Leds::ON);
            Leds::set(Pins::LED_GREEN, Leds::OFF);
        }

        current = mode; 
    }
}

#endif // MODE_H