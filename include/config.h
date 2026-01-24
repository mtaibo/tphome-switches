#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>

struct Config {
  
    // Network configuration
    char wifi_ssid[32];
    char wifi_password[64];
    char mqtt_server[40];
  
    // Blind configuration
    unsigned long up_time;    // ms
    unsigned long down_time;  // ms
    float current_position;   // 0.0 a 100.0

    // Execution data
    bool is_moving;
    unsigned long start_time;
  
    // Preferences
    bool block_buttons;    
    unsigned long short_pulse;
    unsigned long long_pulse;
};

extern Config config; 

void config_init();

#endif