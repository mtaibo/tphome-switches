#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h> // Basic Arduino module
#include <Preferences.h> // Module to save relevant config on flash memory

#define IDENTITY_SIZE 16
#define WIFI_SIZE 32
#define MQTT_SIZE 24

#define NO_PIN 255

// Config, Pref and State structs defined as packed to save memory padding
struct __attribute__((__packed__)) Config {

  // Identification
  char device_id[IDENTITY_SIZE];
  char room[IDENTITY_SIZE];
  char name[IDENTITY_SIZE];

  // WiFi
  char wifi_ssid[WIFI_SIZE];
  char wifi_pass[WIFI_SIZE];

  // MQTT
  char mqtt_ip[MQTT_SIZE];
  char mqtt_user[MQTT_SIZE];
  char mqtt_pass[MQTT_SIZE];
  uint16_t mqtt_port;
};

struct __attribute__((__packed__)) Pref {

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

extern Config config;
extern Pref pref;
extern State state;


void load_config();
void save_config();

void reboot();
void reset_memory();

void config_setup();

#endif // SETTINGS_H
