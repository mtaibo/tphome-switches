#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// ~~~~~~~~~~~~~~~~~~~~        Hardware Logic        ~~~~~~~~~~~~~~~~~~~~

static constexpr uint8_t LED_ON          = HIGH;
static constexpr uint8_t LED_OFF         = LOW;

static constexpr uint8_t LED_RED         = HIGH;
static constexpr uint8_t LED_BLUE        = LOW;

static constexpr uint8_t BUTTON_PRESSED  = LOW;
static constexpr uint8_t BUTTON_RELEASED = HIGH;

static constexpr uint8_t RELAY_ON        = HIGH;
static constexpr uint8_t RELAY_OFF       = LOW;

// ~~~~~~~~~~~~~~~~~~~~            Pinout            ~~~~~~~~~~~~~~~~~~~~

// --- LEDS ---
static constexpr uint8_t PIN_LED_CONFIG = 1;  // Shared with TX/RX - Set this led to OUTPUT mode and HIGH to activate blue leds, and LOW or INPUT mode to deactivate blue leds
static constexpr uint8_t PIN_LED_GREEN  = 0;
static constexpr uint8_t PIN_LED_BOTTOM = 2;
static constexpr uint8_t PIN_LED_MID    = 14;
static constexpr uint8_t PIN_LED_TOP    = 16;

// --- BUTTONS ---
static constexpr uint8_t PIN_BTN_BOTTOM = 5;
static constexpr uint8_t PIN_BTN_MID    = 3;  // Shared with TX/RX
static constexpr uint8_t PIN_BTN_TOP    = 12;

// --- RELAYS ---
static constexpr uint8_t PIN_RELAY_UP   = 13; // Output L1
static constexpr uint8_t PIN_RELAY_DOWN = 15; // Output L3

#endif // PINS_H
