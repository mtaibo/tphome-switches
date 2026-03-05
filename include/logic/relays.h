#ifndef RELAYS_H
#define RELAYS_H

#if defined(DEVICE_HARDWARE_ESP8266)
    #include "hardware/esp8266/hardware.h"
#elif defined(DEVICE_HARDWARE_BK7231N)
    #include "hardware/bk7231n/hardware.h"
#endif

#include "leds.h"

namespace Relays {

    /* Internal variables to manage non blocking relay logic */
    enum Pending { NONE, UP, DOWN };
    static Pending _pending = NONE;
    static uint32_t _timer = 0;

    inline void stop() {
        Hardware::RelayUp::off();
        Hardware::RelayDown::off();
        
        Settings::state.is_moving = false;
        _pending = NONE;

        Leds::set(LED_TOP, Leds::OFF);
        Leds::set(LED_BTM, Leds::OFF);
        Leds::set(LED_MID, Leds::ON, Leds::MEDIUM, 0, 50); 
    }

    inline void update() {

        /* If there is no pending relay, ignore */
        if (_pending == NONE) return;

        if (millis() - _timer >= Defaults::MOTOR_SAFE_TIME) {

            Settings::state.last_time = millis();

            if (_pending == UP) {
                Settings::prefs.inverted_relays ? Hardware::RelayDown::on() : Hardware::RelayUp::on();
            } 
            else if (_pending == DOWN) {
                Settings::prefs.inverted_relays ? Hardware::RelayUp::on() : Hardware::RelayDown::on();
            }
            _pending = NONE;
        }
    }

    inline void moveUp() {

        Hardware::RelayUp::off();
        Hardware::RelayDown::off();

        _pending = UP;
        _timer = millis();

        Leds::set(LED_TOP, Leds::ON);
    }

    inline void moveDown() {

        Hardware::RelayUp::off();
        Hardware::RelayDown::off();

        _pending = DOWN;
        _timer = millis();

        Leds::set(LED_BTM, Leds::ON);
    }
}

#endif