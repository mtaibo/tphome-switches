#ifndef LEDS_H
#define LEDS_H

#if defined(DEVICE_HARDWARE_ESP8266)
    #include "hardware/esp8266/hardware.h"
#elif defined(DEVICE_HARDWARE_BK7231N)
    #include "hardware/bk7231n/hardware.h"
#endif

namespace Leds {

    /* Enums to simplify code */
    enum Mode { OFF, ON, BLINK, BLUE = OFF, RED = ON };
    enum Speed { SLOW = 100, MEDIUM = 50, FAST = 25 }; // 1s, 0.5s, 0.25s

    /* Struct that contains every relevant value of each led */
    struct LedState {
        uint8_t pin;
        Mode mode;
        bool state;
        uint16_t interval;
        uint16_t counter;
        uint16_t lastTime;
        uint16_t offTime;
    };

    /* Array that stores every active led */
    inline LedState _states[4] = {
        {Pins::NO_PIN}, {Pins::NO_PIN}, {Pins::NO_PIN}, {Pins::NO_PIN}  // Initialize _states with no pins
    }; 

    /* Function that provides the indicated led the indicated state */
    inline void set(uint8_t pin, Mode mode, Speed speed = MEDIUM, uint16_t blinks = 0, uint16_t duration = 0) {

        LedState* slot = nullptr; // Pointer to the slot to be modified

        /* Looking for the pin slot, or an empty slot if pin not found */
        for (auto &state : _states) {
            if (state.pin == pin) { slot = &state; break; }
            if (state.pin == Pins::NO_PIN && !slot) slot = &state;
        }

        /* Adjust every variable of the state */
        slot -> pin = pin;
        slot -> mode = mode;
        slot -> state = (mode == ON);
        slot -> interval = speed;
        slot -> counter = blinks * 2;
        slot -> lastTime = (uint16_t)(millis() / 10);
        slot -> offTime = (duration > 0) ? (uint16_t)(millis() / 10) + duration : 0;

        /* Hardware interaction */
        Hardware::setState(pin, slot->state);
    }

    inline void off() { // Turn every pin off
        for (auto &state : _states) {
            if (state.pin != Pins::NO_PIN) {Hardware::setState(state.pin, false); state.mode = OFF;}
        }
    }

    /* Function that controls every led state */
    inline void update() {
        uint16_t now = (uint16_t)(millis() / 10); // now: Variable to prevent calculating millis() each time
        for (auto &state : _states) { // Loop to update every active led

            /* If there is no pin on that led state slot, continue with other led */
            if (state.pin == Pins::NO_PIN) continue;

            /* Duration control */
            if (state.offTime > 0 && (int16_t)(now - state.offTime) >= 0) {
                Hardware::setState(state.pin, false);
                state.mode = OFF; continue;
            } 

            /* If the pin mode is not blink, skip blinking control */
            if (state.mode != BLINK) continue;

            /* Blinking control */
            if (now - state.lastTime >= state.interval) {

                state.lastTime = now;

                state.state = !state.state;
                Hardware::setState(state.pin, state.state);

                if (state.counter > 0) {
                    state.counter--;
                    if (state.counter == 0) {
                        state.mode = OFF;
                        Hardware::setState(state.pin, false);
                    }
                }
            }
        }
    }
}

#endif // LEDS_H