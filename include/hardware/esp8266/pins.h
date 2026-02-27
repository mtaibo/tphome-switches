#ifndef PINS_H
#define PINS_H

// clang-format off

#include <stdint.h> // Lighter library than Arduino just for variable types

// ~~~~~~~~~~~~~~~~~~~~        Hardware Logic        ~~~~~~~~~~~~~~~~~~~~

namespace HW_Logic {

  static constexpr uint8_t LED_ACTIVE   = HIGH;
  static constexpr uint8_t LED_IDLE     = LOW;

  static constexpr uint8_t BTN_ACTIVE   = LOW;
  static constexpr uint8_t BTN_IDLE     = HIGH;

  static constexpr uint8_t RELAY_ACTIVE = HIGH;
  static constexpr uint8_t RELAY_IDLE   = LOW;
}

// ~~~~~~~~~~~~~~~~~~~~            Pinout            ~~~~~~~~~~~~~~~~~~~~

namespace Pins {

  // --- LEDS ---
  static constexpr uint8_t LED_CONFIG = 1;  // Shared with TX/RX - Set this led to OUTPUT mode and HIGH to activate blue leds, and LOW or INPUT mode to deactivate blue leds
  static constexpr uint8_t LED_GREEN  = 0;
  static constexpr uint8_t LED_BTM    = 2;
  static constexpr uint8_t LED_MID    = 14;
  static constexpr uint8_t LED_TOP    = 16;

  // --- BUTTONS ---
  static constexpr uint8_t BTN_BTM    = 5;
  static constexpr uint8_t BTN_MID    = 3;  // Shared with TX/RX
  static constexpr uint8_t BTN_TOP    = 12;

  // --- RELAYS ---
  static constexpr uint8_t RELAY_UP   = 13; // Output L1
  static constexpr uint8_t RELAY_DOWN = 15; // Output L3
}

// clang-format on

#endif // PINS_H
