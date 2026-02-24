#ifndef DEFAULT_H
#define DEFAULT_H

// ------- WiFi / MQTT Credentials  ---------

#include "credentials.h"

// ---------     Identification     ---------

#define DEF_DEVICE_ID               "B364859"
#define DEF_ROOM          "secondary-bedroom"
#define DEF_NAME                      "blind"

// ---------     Preferences        ---------

#define DEF_UP_TIME                     30000
#define DEF_DOWN_TIME                   30000
#define DEF_DOWN_POSITION                  20 // This value is used to partially close the blind to a memorised position

#define DEF_MOTOR_SAFE_TIME              1000 // This value is a safe time for the motor between going up and down
#define DEF_STOP_LED_TIME                 500

#define DEF_SHORT_PULSE                  3000
#define DEF_LONG_PULSE                  10000

// ---------     Default State      ---------

#define DEF_CURRENT_POSITION              100
#define DEF_NEXT_POSITION                 100

#define DEF_ACTIVE_RELAY               NO_PIN
#define DEF_ACTIVE_LED                 NO_PIN

#define DEF_PENDING_RELAY              NO_PIN
#define DEF_PENDING_LED                NO_PIN

#define DEF_IS_MOVING                   false
#define DEF_IS_WAITING                  false
#define DEF_IS_BLINKING                 false
#define DEF_PAUSE_CONTROL               false

#endif
