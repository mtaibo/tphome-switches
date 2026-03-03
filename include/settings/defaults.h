#ifndef DEFAULTS_H
#define DEFAULTS_H

/* Char size definitions */
#define IDENTITY_SIZE 16
#define WIFI_SIZE     32
#define MQTT_SIZE     24

#define NO_PIN 255 // Max value on 8 bits


// ------- WiFi / MQTT Credentials  ---------

#include "credentials.h"

// ---------     Identification     ---------

static constexpr char DEF_DEVICE_ID[IDENTITY_SIZE] = "B364859";
static constexpr char DEF_ROOM[IDENTITY_SIZE]      = "bedroom";
static constexpr char DEF_NAME[IDENTITY_SIZE]      = "blind";

// ---------     Preferences        ---------

/* Buttons */
static constexpr uint16_t DEF_SHORT_PULSE     = 100; //  1.00 seconds
static constexpr uint16_t DEF_LONG_PULSE      = 500; //  5.00 seconds

/* Blinds */
static constexpr uint16_t DEF_UP_TIME         = 4000; // 40.00 seconds
static constexpr uint16_t DEF_DOWN_TIME       = 4000; // 40.00 seconds
static constexpr uint16_t DEF_DOWN_POSITION   = 2000; // 20%              -   This value is used to partially close the blind to a memorised position

static constexpr uint16_t DEF_STOP_LED_TIME   = 50; //  0.50 seconds    -   This is a timer for the mid led (stop led) to turn it off after clicking on it
static constexpr uint16_t DEF_MOTOR_SAFE_TIME = 100; //  1.00 seconds    -   This value is a safe time to prevent overheating the motor by changing direction really fast

/* Lights */

// ---------       States           ---------

/* Blinds */
static constexpr uint16_t DEF_CURRENT_POSITION = 5000; // 50%  -  Initialize blind position at 50% to allow user to move blind up and down to begin calibration
static constexpr uint16_t DEF_NEXT_POSITION = 5000; // 50%

static constexpr uint8_t DEF_ACTIVE_RELAY = NO_PIN;
static constexpr uint8_t DEF_ACTIVE_LED = NO_PIN;

static constexpr uint8_t DEF_PENDING_RELAY = NO_PIN;
static constexpr uint8_t DEF_PENDING_LED = NO_PIN;

static constexpr bool DEF_IS_MOVING = false;
static constexpr bool DEF_IS_WAITING = false;
static constexpr bool DEF_IS_BLINKING = false;
static constexpr bool DEF_PAUSE_CONTROL = false;

/* Lights */

void defaults();

#endif // DEFAULTS_H
