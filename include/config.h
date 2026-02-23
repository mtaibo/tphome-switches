#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> // Basic Arduino module
#include <Preferences.h> // Module to save relevant config on flash memory

// Config struct defined as packed to save memory padding
struct __attribute__((__packed__)) Config {

  // WiFi
  char wifi_ssid[24];
  char wifi_pass[32];

  // MQTT
  char mqtt_server[20];
  char mqtt_user[15];
  char mqtt_pass[15];
  uint16_t mqtt_port;

  // Identification
  char device_id[16];
  char room[16];
  char name[16];

  // Preferences
  uint16_t up_time;
  uint16_t down_time;
  uint16_t down_position;

  uint16_t mid_led_time;
  uint16_t motor_safe_time;

  uint16_t short_pulse;
  uint16_t long_pulse;

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

extern Config config;

void load_config();
void save_config();

void reboot();
void reset_memory();

void config_setup();

#endif
