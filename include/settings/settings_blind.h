#ifndef SETTINGS_BLIND_H
#define SETTINGS_BLIND_H

#include <stdint.h>

struct __attribute__((__packed__)) Prefs {

    uint16_t up_time;
    uint16_t down_time;

    uint16_t mid_led_time;
    uint16_t motor_safe_time;

    uint16_t short_pulse;
    uint16_t long_pulse;

    uint16_t down_position;
};

struct __attribute__((__packed__)) State {

    // Execution
    uint16_t current_position;
    uint16_t next_position;

    uint8_t active_relay;
    uint8_t active_led;

    uint8_t pending_relay;
    uint8_t pending_led;

    // States
    bool is_moving;
    bool is_waiting;
    bool is_blinking;
    bool pause_control;
};

#endif // SETTINGS_BLIND_H
