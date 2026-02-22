#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

struct Config {

    // Device ID
    char device_id[16];

    // WIFI configuration
    char wifi_ssid[32];
    char wifi_pass[64];

    // MQTT configuration
    char mqtt_server[32];
    unsigned int mqtt_port;

    char mqtt_user[32];
    char mqtt_pass[32];
  
    // Topic configuration
    char type[32];
    char room[32];
    char name[32];

    char set_topic[128];
    char state_topic[128];
    char admin_set_topic[64];
    char admin_state_topic[64];

    // Blind configuration
    unsigned long up_time;
    unsigned long down_time;

    float down_position;
    float current_position;

    // Execution data
    bool is_moving;
    bool is_waiting;
    bool network_timeout;

    float next_position;

    int active_relay;
    int active_led;
    int pending_relay;
    int pending_led;

    // Time execution data
    bool pause_control;
    unsigned long stop_time;
    unsigned long stop_led_time;
    unsigned long current_time_limit;
    unsigned long last_cycle_time;

    // Blinking control
    bool is_blinking;
    int blinking_led;
    int blinking_state;
    unsigned long blink_time;
    unsigned long last_blink;

    // Preferences
    unsigned long mid_led_time;
    unsigned long motor_safe_time;

    unsigned long short_pulse;
    unsigned long long_pulse;
};

extern Config config;

// Config file managment
void load_config();
void save_config();

void reboot();
void reset_memory();

void config_setup();

#endif
