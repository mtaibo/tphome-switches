#include <credentials.h>
#include <config.h>
#include <pins.h>

Config config;

void config_setup() {

    // This both lines below activate the pin to allow
    // the blue leds to turn on, otherwise, they'll be blocked
    pinMode(CONFIG_LED, OUTPUT);
    digitalWrite(CONFIG_LED, HIGH);

    // Loop to turn every output pin to output mode
    int output_pins[] = {LED_TOP, LED_MID, LED_BOTTOM, LED_GREEN, RELAY_UP, RELAY_DOWN};
    for (int i=0; i<6; i++) {
        pinMode(output_pins[i], OUTPUT);
        digitalWrite(output_pins[i], LOW);
    }

    // Device ID
    strcpy(config.device_id, DEVICE_ID);

    // Network configuration
    strcpy(config.wifi_ssid, WIFI_SSID);
    strcpy(config.wifi_password, WIFI_PASS);

    strcpy(config.mqtt_server, MQTT_SERVER);
    config.mqtt_server_port = MQTT_SERVER_PORT;

    strcpy(config.mqtt_user, MQTT_USER);
    strcpy(config.mqtt_pass, MQTT_PASS);

    config_default();
}


void config_default() {

    // ! VARIABLES THAT CAN BE MODIFIED

    // Topic setup
    strcpy(config.type, "blinds");
    strcpy(config.room, "lounge");
    strcpy(config.name, "big-blind");

    // Blind configuration
    config.up_time = 40000;
    config.down_time = 40000;
    config.current_position = 50.0;

    // ! VARIABLES THAT ARE JUST DEFAULT SETTINGS

    // Execution data
    config.is_moving = false;
    config.is_waiting = false;

    // Blinking config
    config.blink_time = 500;
    config.blinking_led = -1;
    config.is_blinking = false;
    config.last_blink = 0;

    // Preferences
    config.mid_led_time = 1000;
    config.motor_safe_time = 1000;

    config.short_pulse = 3000;
    config.long_pulse = 10000;
}