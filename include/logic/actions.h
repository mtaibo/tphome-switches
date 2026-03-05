#ifndef ACTIONS_H
#define ACTIONS_H

#include "buttons.h"
#include "position.h"

namespace Actions {
    inline void check() {

        #if defined(DEVICE_TYPE_BLIND)

            if (auto action = Buttons::getAction(BTN_TOP)) {
                if (action == Buttons::SHORT) Position::set((uint16_t)10000);
                else if (action == Buttons::MEDIUM) Position::set(Settings::prefs.down_position);
                else if (action == Buttons::LONG) Position::set(Settings::state.current_position);
            }

            else if (auto action = Buttons::getAction(BTN_MID)) {
                if (action == Buttons::SHORT) Relays::stop();
                else if (action == Buttons::MEDIUM) Position::set(Settings::state.current_position);
                else if (action == Buttons::LONG) Position::set(Settings::state.current_position);
            }

            else if (auto action = Buttons::getAction(BTN_BTM)) {

                if (action == Buttons::SHORT) {
                    if (Settings::state.current_position >= Settings::prefs.down_position) {
                        Position::set(Settings::prefs.down_position);
                    } else Position::set((uint16_t)0);
                }

                else if (action == Buttons::MEDIUM) Position::set((uint16_t)0);
                else if (action == Buttons::LONG) Position::set(Settings::state.current_position);
            }

        #endif
    }
}

#endif