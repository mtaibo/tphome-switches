#ifndef CONFIG
#define CONFIG

#include <Arduino.h>

struct Config {
  
    // WIFI configuration
    char wifi_ssid[32];
    char wifi_password[64];

    // MQTT configuration
    char mqtt_server[40]; // Raspberry IP
    char mqtt_user[40];
    char mqtt_pass[40];
  
    // Blind configuration
    unsigned long up_time;    // ms
    unsigned long down_time;  // ms
    float current_position;   // 0.0 a 100.0

    // Execution data
    bool is_moving;
    bool is_waiting;

    bool pause_control;

    unsigned long stop_time; 
    unsigned long stop_led_time;

    unsigned long current_limit;

    // Information about running relay and led
    int active_relay;  
    int active_led;          
    int pending_relay;
    int pending_led;

    // Blinking control
    bool is_blinking;
    int blinking_led;
    int blinking_state;
    unsigned long last_blink;
  
    // Preferences
    unsigned long mid_led_time;
    unsigned long motor_safe_time;

    unsigned long short_pulse;
    unsigned long long_pulse;
};

extern Config config; 

void config_setup();
void config_default();

#endif