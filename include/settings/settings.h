#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h> // Module for uint8_t and uint16_t
#include "defaults.h"

/* Module that defines which settings should include,
 * the blinds one or the lights one. */
#if defined(DEVICE_TYPE_BLIND)
    #include "settings_blind.h"
#elif defined(DEVICE_TYPE_LIGHT)
    #include "settings_light.h"
#endif


namespace Settings {

    struct __attribute__((__packed__)) Config {

        // Identity
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

    extern Config config;
    extern Prefs prefs;
    extern State state;

    void reboot();
    void reset();
    void defaults();

    void saveState();

    void save();
    void load();
    void setup();
}

#endif // SETTINGS_H