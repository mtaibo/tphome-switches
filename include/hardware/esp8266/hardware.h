#ifndef HARDWARE_H
#define HARDWARE_H

#include "pinout.h"
#include "defaults.h"
#include "driver_input.h"
#include "driver_output.h"

namespace Hardware {

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