#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <stdint.h>
#include <string.h>

/* ---  Network  ---- */
#include "credentials.h"

/* --- Config Sizes --- */
#define IDENTITY_SIZE 16
#define WIFI_SIZE     32
#define MQTT_SIZE     24

#define NO_PIN        255

namespace Defaults {

    /* --- Identification --- */
    static constexpr char ID []   = "B364859";
    static constexpr char ROOM [] = "bedroom";
    static constexpr char NAME [] = "blind";

    /* --- Timings & Preferences --- */
    static constexpr uint16_t UP_TIME          = 1000; // 10.00s
    static constexpr uint16_t DOWN_TIME        = 1000; // 10.00s
    static constexpr uint16_t DOWN_POSITION    = 2000; // 20.00%
    static constexpr bool INVERTED_RELAYS      = false;

    static constexpr uint16_t MOTOR_SAFE_TIME  = 50;   // 0.50s
    static constexpr uint16_t LONG_PULSE       = 500;  // 5.00s
    static constexpr uint16_t SHORT_PULSE      = 200;  // 2.00s

    /* --- Initial State --- */
    static constexpr uint16_t START_POSITION   = 10000; // 100.00%
    static constexpr bool IS_MOVING            = false;
    static constexpr bool IS_WAITING           = false;
}

#endif