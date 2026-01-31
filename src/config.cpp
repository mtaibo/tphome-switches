#include <config.h>
#include <credentials.h>

Config config;

void config_setup() {

    config_default();

}


void config_default() {

    // Network configuration
    strcpy(config.wifi_ssid, WIFI_SSID);
    strcpy(config.wifi_password, WIFI_PASS);

    strcpy(config.mqtt_server, MQTT_SERVER);
    strcpy(config.mqtt_user, MQTT_USER);
    strcpy(config.mqtt_pass, MQTT_PASS);

    // Blind configuration
    config.up_time = 30000;
    config.down_time = 25000;
    config.current_position = 50.0;

    // Execution data
    config.is_moving = false;
    config.is_waiting = false;

    // Blinking config
    config.blinking_led = -1;
    config.is_blinking = false;
    config.last_blink = 0;

    // Preferences
    config.mid_led_time = 1000;
    config.motor_safe_time = 1000;

    config.short_pulse = 3000;
    config.long_pulse = 10000;
}