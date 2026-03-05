#ifndef POSITION_H
#define POSITION_H

#include "relays.h"
#include "settings.h"

namespace Position {

    inline void update() {
        auto& s = Settings::state;
        auto& p = Settings::prefs;

        /* If blind is not moving, there's nothing to update */
        if (!s.is_moving) return;

        /* Getting current movement direction */
        bool goingDown = (s.next_position > s.current_position);
        
        /* Calculating pure delta time from last time and now */
        uint32_t now = millis();
        uint32_t dt = now - s.last_time;
        s.last_time = now;

        /* Calculating how much the blind has moved since last time */
        uint32_t totalTime = (goingDown ? p.down_time : p.up_time) * 10;
        uint16_t deltaPos = (dt * 10000) / totalTime;

        if (goingDown) {

            /* Calculating new current blind position */
            s.current_position += deltaPos;

            /* Checking if the blind arrived the targeted position */
            if (s.current_position >= s.next_position) {s.current_position = s.next_position; Relays::stop();}

        } else { // goingUp

            /* Calculating new current blind position, checking negative numbers */
            if (s.current_position > deltaPos) s.current_position -= deltaPos;
            else s.current_position = 0;
            
            /* Checking if the blind arrived the targeted position */
            if (s.current_position <= s.next_position) {s.current_position = s.next_position; Relays::stop();}
        }
    }

    inline void set(uint16_t target) {
        auto& s = Settings::state;
        if (target == s.current_position) return;

        s.next_position = target;
        s.is_moving = true;

        if (target > s.current_position) Relays::moveDown();
        else Relays::moveUp();
    }
}
#endif