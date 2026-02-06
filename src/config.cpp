#include <LittleFS.h>
#include <ArduinoJson.h>

#include "credentials.h"
#include "config.h"
#include "pins.h"

Config config;

void pin_setup() {

   // This both lines below activate the pin to allow
    // the blue leds to turn on, otherwise, they'll be blocked
    pinMode(CONFIG_LED, OUTPUT);
    digitalWrite(CONFIG_LED, HIGH);

    // Loop to turn every output pin to output mode and low
    int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
        digitalWrite(output_pins[i], LOW);
    }
}

bool manage_json(int mode) {
    
    // Get the config file from the flash memory
    File configFile = LittleFS.open("/config.json", "r");
    if (!configFile) return false;

    // Get the json on "doc" from the file that was opened
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, configFile);

    configFile.close(); // Close the file that was opened by LittleFS to free memory
    if (error) return false; // If json is corrupted, return false after closing the configFile

    // TODO: Implement the reading interaction and saving its values con config struct
    // TODO: Implement mode selector from int mode input

    return true;
}

void config_default() {

    // Device ID
    strcpy(config.device_id, DEVICE_ID);

    // Network configuration
    strcpy(config.wifi_ssid, WIFI_SSID);
    strcpy(config.wifi_password, WIFI_PASS);

    strcpy(config.mqtt_server, MQTT_SERVER);
    config.mqtt_server_port = MQTT_SERVER_PORT;

    strcpy(config.mqtt_user, MQTT_USER);
    strcpy(config.mqtt_pass, MQTT_PASS);


    // ! VARIABLES THAT CAN BE MODIFIED

    // Topic setup
    strcpy(config.type, "blinds");
    strcpy(config.room, "lounge");
    strcpy(config.name, "big-blind");

    // Blind configuration
    config.up_time = DEFAULT_UP_TIME;
    config.down_time = DEFAULT_DOWN_TIME;
    config.current_position = 50.0;

    // ! VARIABLES THAT ARE JUST EXECUTION SETTINGS

    // Execution data
    config.is_moving = false;
    config.is_waiting = false;

    // Blinking config
    config.blink_time = 500;
    config.is_blinking = false;
    config.last_blink = 0;

    // Preferences
    config.mid_led_time = 1000;
    config.motor_safe_time = 1000;

    config.short_pulse = 3000;
    config.long_pulse = 10000;
}

void config_setup() {
    pin_setup();
    if (!manage_json(0)) config_default();
}