#ifndef HARDWARE_H
#define HARDWARE_H

#include "defaults.h"
#include "hardware/esp8266/pinout.h"
#include "hardware/esp8266/driver_input.h"
#include "hardware/esp8266/driver_output.h"

namespace Hardware { // Hardware for ESP8266

    // --- BUTTONS ---
    static constexpr uint16_t MAX_TIME = DEF_LONG_PULSE;

    using BtnBtm = InputPin<BTN_BTM, MAX_TIME>;
    using BtnMid = InputPin<BTN_MID, MAX_TIME>;
    using BtnTop = InputPin<BTN_TOP, MAX_TIME>;

    // --- LEDS ---
    using LedConfig = OutputPin<LED_CONFIG>;
    using LedGreen  = OutputPin<LED_GREEN>;
    using LedBtm    = OutputPin<LED_BTM>;
    using LedMid    = OutputPin<LED_MID>;
    using LedTop    = OutputPin<LED_TOP>;

    // --- RELAYS ---
    using RelayUp   = OutputPin<RELAY_UP>;
    using RelayDown = OutputPin<RELAY_DOWN>;

    // --- LIGHT CONTROL ---
    static inline void enableLight() {LedConfig::setup(); LedConfig::on();}
    static inline void disableLight() {LedConfig::off();}

    // --- LED CONTROL ---
    static void setState(uint8_t pin, bool state) {
        switch (pin) {
            case LED_GREEN: state ? LedGreen::on() : LedGreen::off(); break;
            case LED_BTM: state ? LedBtm::on() : LedBtm::off(); break;
            case LED_MID: state ? LedMid::on() : LedMid::off(); break;
            case LED_TOP: state ? LedTop::on() : LedTop::off(); break;
        }
    }

    // --- GLOBAL SETUP ---
    static void setup() {

        LedGreen::setup();
        LedBtm::setup();
        LedMid::setup();
        LedTop::setup();

        BtnBtm::setup();
        BtnMid::setup();
        BtnTop::setup();

        RelayUp::setup();
        RelayDown::setup();

        enableLight();
    }
}

#endif // HARDWARE_H