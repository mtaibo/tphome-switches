#ifndef RELAYS_H
#define RELAYS_H

#if defined(DEVICE_HARDWARE_ESP8266)
    #include "hardware/esp8266/hardware.h"
#elif defined(DEVICE_HARDWARE_BK7231N)
    #include "hardware/bk7231n/hardware.h"
#endif

#include "leds.h"

namespace Relays {

    inline void stop() {
        Hardware::RelayUp::off();
        Hardware::RelayDown::off();

        Leds::set(LED_TOP, Leds::OFF);
        Leds::set(LED_BTM, Leds::OFF);

        Leds::set(LED_MID, Leds::ON, Leds::MEDIUM, 0, 50); 
    }

    inline void moveUp() {
        Hardware::RelayDown::off();
        delay(DEF_MOTOR_SAFE_TIME);
        Hardware::RelayUp::on();
        Leds::set(LED_TOP, Leds::ON);
    }

    inline void moveDown() {
        Hardware::RelayUp::off();
        delay(DEF_MOTOR_SAFE_TIME);
        Hardware::RelayDown::on();
        Leds::set(LED_BTM, Leds::ON);
    }
}

#endif