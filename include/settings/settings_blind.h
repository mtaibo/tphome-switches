#ifndef SETTINGS_BLIND_H
#define SETTINGS_BLIND_H

#include <stdint.h>

struct __attribute__((__packed__)) Prefs {
    uint16_t up_time;
    uint16_t down_time;
    uint16_t down_position;
    bool inverted_relays;
};

struct __attribute__((__packed__)) State {
    uint32_t last_time;
    uint16_t current_position;
    uint16_t next_position;
    bool is_moving;
};

#endif // SETTINGS_BLIND_H
