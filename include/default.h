#ifndef DEFAULT_H
#define DEFAULT_H

// ~~~~~~~~~~~~~~~~~~~~         Measures         ~~~~~~~~~~~~~~~~~~~~~~~
//
//  Variables with this measures, time or pos are 16 bits unsigned ints
//
// Time      -  Measured in centiseconds (1.00s = 100   | 25.12s = 2512)
// Position  -  Measured                 (10.00% = 1000 | 26.37% = 2637)
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// ------- WiFi / MQTT Credentials  ---------

#include "credentials.h" // Only a credentials.example.h is on this repository to prevent showing private passwords

// ---------     Identification     ---------

#define DEF_DEVICE_ID               "B364859" // This device ID must follow a standard, beginning with B for Blind or L for Light, and having 6 numbers after that letter
#define DEF_ROOM                    "bedroom"
#define DEF_NAME                      "blind"

// ---------     Preferences        ---------

#define DEF_UP_TIME                      4000 // 40.00 seconds
#define DEF_DOWN_TIME                    4000 // 40.00 seconds
#define DEF_DOWN_POSITION                2000 // 20%              -   This value is used to partially close the blind to a memorised position

#define DEF_MOTOR_SAFE_TIME              0100 //  1.00 seconds    -   This value is a safe time for the motor between going up and down
#define DEF_STOP_LED_TIME                0050 //  0.50 seconds

#define DEF_SHORT_PULSE                  0300 //  3.00 seconds
#define DEF_LONG_PULSE                   1000 // 10.00 seconds

// ---------     Default State      ---------

#define DEF_CURRENT_POSITION             5000 // 50%
#define DEF_NEXT_POSITION                5000 // 50%

#define DEF_ACTIVE_RELAY               NO_PIN
#define DEF_ACTIVE_LED                 NO_PIN

#define DEF_PENDING_RELAY              NO_PIN
#define DEF_PENDING_LED                NO_PIN

#define DEF_IS_MOVING                   false
#define DEF_IS_WAITING                  false
#define DEF_IS_BLINKING                 false
#define DEF_PAUSE_CONTROL               false

#endif
